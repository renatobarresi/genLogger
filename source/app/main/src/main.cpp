/**
 * @file main.cpp
 * @author Renato Barresi (renatobarresi@gmail.com)
 * @brief Main application file for the genLogger project.
 *
 * This file contains the main entry point and the superloop for the genLogger,
 * a generic datalogger application. It initializes and orchestrates the various
 * subsystems of the application.
 *
 * The application has five main subsystems:
 *  1) Configuration subsystem: Handles user interaction for device configuration.
 *  2) Measurement subsystem: Manages sensor data acquisition.
 *  3) Processing subsystem: Processes raw sensor data.
 *  4) Storage subsystem: Manages data logging to internal storage.
 *  5) Network subsystem: Handles network communication, such as sending data to a server.
 *
 * The macro `TARGET_MICRO` must be passed as a compiler definition for target microcontroller builds. When defined,
 * components related to the target board are included.
 *
 * @version 0.1
 * @date 2025-01-12
 *
 * @copyright Copyright (c) 2025
 *
 */

////////////////////////////////////////////////////////////////////////
//							    Includes
////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "config_manager.hpp"
#include "debug_log.hpp"
#include "httpClient.hpp"
#include "internalStorage_component.hpp"
#include "logger_manager.hpp"
#include "networkManager.hpp"
#include "processing_manager.hpp"
#include "serialHandler.hpp"
#include "terminal_component.hpp"
#include "utilities.hpp"
#include "virtualRTC.hpp"
#include "virtualTimer.hpp"

// Includes related to the used board
#ifdef TARGET_MICRO
#include "ethernet.h"
#include "init.h"
#include <new>

void* operator new(std::size_t count) = delete; // Make sure no library that uses dynamic allocation is being used
#endif

////////////////////////////////////////////////////////////////////////
//							Constants/Macros
////////////////////////////////////////////////////////////////////////

constexpr char loggerDefaultIP[]	  = "123.123.123.123";
constexpr char loggerDefaultNetmask[] = "255.255.255.0";
constexpr char loggerDefaultGateway[] = "123.123.123.1";
constexpr char httpServerIP[]		  = "127.0.0.1:8080";

////////////////////////////////////////////////////////////////////////
//					       Private variables
////////////////////////////////////////////////////////////////////////

/**
 * @brief Flag to signal the execution of the measurement task.
 * @details This flag is set by a timer (or simulated SysTick) and cleared after the task runs.
 */
static bool runMeasurementTask = true;
/**
 * @brief Period for the measurement task in milliseconds.
 */
static uint32_t measurementTaskPeriod = utilities::MS_IN_ONE_MINUTE;

#ifdef TARGET_MICRO
struct hardwareTimeouts			 taskMeasurementControl{&measurementTaskPeriod, 0};
std::array<hardwareTimeouts*, 2> taskControlContainer{&taskMeasurementControl, nullptr};
#endif

////////////////////////////////////////////////////////////////////////
//							    Classes
////////////////////////////////////////////////////////////////////////

virtualRTC rtc;
/** @brief Terminal state machine for user configuration via serial interface. */
terminalStateMachine terminalOutput(&rtc);
/** @brief Component for handling metadata storage on the internal filesystem. */
internalStorageComponent internalStorage;
/** @brief Mediator for the configuration subsystem, connecting terminal and storage. */
configManager loggerConfig(&terminalOutput, &internalStorage);
/** @brief Manager for processing data from various sensors. */
processingManager<sensor::davisPluviometer, sensor::anemometerDavis> myProcessingManager(rtc);
/** @brief Manager for logging processed data to files. */
loggerManager myLoggerManager;
/** @brief Manager for network connectivity. */
network::networkManager loggerNetworkManager(loggerDefaultIP, loggerDefaultNetmask, loggerDefaultGateway);
/** @brief HTTP client for sending data to a remote server. */
network::httpClient loggerHttpClient(&loggerNetworkManager, httpServerIP);

////////////////////////////////////////////////////////////////////////
//							Functions definitions
////////////////////////////////////////////////////////////////////////

// TODO: Add RTOS
/** @brief Task to handle device configuration via the terminal interface. */
static void configurationTask();
/** @brief Task to handle logging of sensor data to the filesystem. */
static void loggerTask();
/** @brief Task to perform sensor measurements and data processing. */
static void measurementTask();
/** @brief Task to handle network-related activities, like sending data. */
static void networkTask();

#ifndef TARGET_MICRO
/**
 * @brief Simulates a microcontroller SysTick Handler for non-target builds.
 * @details This function is called periodically by a simulated systick timer. It increments a
 * tick counter and sets the `runMeasurementTask` flag when the measurement period elapses.
 */
void myTickHandler();
#endif

////////////////////////////////////////////////////////////////////////
//							Functions Declaration
////////////////////////////////////////////////////////////////////////

/**
 * @brief Main function, the entry point of the application.
 *
 * @details Initializes all the subsystems, including hardware (on target), internal storage,
 * serial handler, network manager, and the various application-level managers.
 * After initialization, it enters an infinite superloop where it calls the task functions.
 * @return int Returns 0 on successful execution (though it never returns in this application).
 */
int main()
{
	/* Initialization */
#ifdef TARGET_MICRO
	stm32f429_init(taskControlContainer.data());
#else
	systick::startSystickSimulation(myTickHandler);
#endif

	debug::log<true, debug::logLevel::LOG_ALL>("\r\n\r\n-----> Running logger APP <-----\r\n\r\n");

	if (false == serialHandlerInit())
	{
		debug::log<true, debug::logLevel::LOG_ERROR>("Error initializing serial handler\r\n");
		while (1);
	}

	if (false == internalStorage.initFS())
	{
		debug::log<true, debug::logLevel::LOG_ERROR>("Error initializing internal storage\r\n");
		while (1);
	}

#ifdef TARGET_MICRO
	// Get MAC address from the hardware driver and provide it to the network manager
	uint8_t mac_address[6];
	eth_get_mac_address(mac_address);
	loggerNetworkManager.setMacAddress(mac_address);
#endif
	// if (false == loggerNetworkManager.init())
	// {
	// 	debug::log<true, debug::logLevel::LOG_ERROR>("Error initializing network manager\r\n");
	// 	while (1);
	// }

	/* Configuration */
	measurementTaskPeriod = internalStorage.getMeasurementPeriod();

	terminalOutput.init(terminalState::initState);
	terminalOutput.handler(terminalSignal::ENTRY, nullptr);

	myProcessingManager.init();
	myProcessingManager.setObserver(&myLoggerManager);
	myProcessingManager.setObserver(&loggerHttpClient);

	myLoggerManager.init();

	loggerHttpClient.setMailBox(myProcessingManager.getSensorInfoBuff());

	/* Super loop | TODO RTOS */
	while (1)
	{
		configurationTask();
		measurementTask();
		loggerTask();
		networkTask();
	}

#ifndef TARGET_MICRO
	systick::stopSystickSimulation();
#endif

	return 0;
}

/**
 * @brief Handles the configuration subsystem tasks.
 * @details Checks for incoming serial data for user commands, dispatches signals to the terminal
 * state machine, and updates system parameters if the metadata has been changed.
 */
void configurationTask()
{
	terminalSignal signal;
	char		   configBuff[96];
	char*		   pPayload = nullptr;

	// Check if data is available in serial port
	bool process_input = serialHandler();

	// If data is available, process it and generate signals to terminal SM
	if (process_input)
	{
		// Passing signals to handler
		signal = getTerminalSignal();

		if (signal == terminalSignal::NONE)
		{
			return;
		}

		pPayload = getSerialBuffer(configBuff, 96);
		terminalOutput.handler(signal, pPayload);

		clearSerialBuffer();
	}

	// If changes in metadata, parse them
	if (internalStorage.getMetadataUpdatedFlag() == true)
	{
		//TODO all metadata relevant parameters
		measurementTaskPeriod = internalStorage.getMeasurementPeriod();
	}
}

/**
 * @brief Executes the measurement and data processing task.
 * @details When the `runMeasurementTask` flag is set, this function triggers the processing manager to take measurements, format the data, and notify observers.
 */
void measurementTask()
{
	if (runMeasurementTask == true)
	{
		myProcessingManager.takeMeasurements();
		myProcessingManager.formatData();
		myProcessingManager.notifyObservers();

		runMeasurementTask = false;
	}
}

/**
 * @brief Executes the data logging task.
 * @details If the logger manager has new data available, this function calls its handler to write the data to storage.
 */
void loggerTask()
{
	if (true == myLoggerManager.getAvailableDataFlag())
	{
		debug::log<true, debug::logLevel::LOG_ALL>("Running logger task\r\n");
		myLoggerManager.handler();
	}
}

/**
 * @brief Executes the network task.
 * @details If the HTTP client has data to send, this function attempts to POST it to the configured server.
 */
static void networkTask()
{
	if (true == loggerHttpClient.getAvailableDataFlag())
	{
		debug::log<true, debug::logLevel::LOG_ALL>("Running network task\r\n");
		// if (false == loggerHttpClient.postSensorData())
		// {
		// 	// todo
		// }

		loggerHttpClient.setRunTaskFlag(false);
	}
}

#ifndef TARGET_MICRO
/**
 * @brief Simulates a microcontroller SysTick Handler for non-target builds.
 * @details This function is called periodically by a simulated systick timer. It increments a
 * tick counter and sets the `runMeasurementTask` flag when the measurement period elapses.
 */
void myTickHandler()
{
	static uint64_t tickCount = 0;
	tickCount++;
	if ((tickCount % measurementTaskPeriod) == 0)
	{
		runMeasurementTask = true;
	}
}
#endif