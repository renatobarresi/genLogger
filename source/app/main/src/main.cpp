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
#include "ADS1115_wrapper.hpp"
#include "aht21_wrapper.hpp"
#include "config_manager.hpp"
#include "debug_log.hpp"
#include "httpClient.hpp"
#include "internalStorage_component.hpp"
#include "logger_manager.hpp"
#include "networkManager.hpp"
#include "processing_manager.hpp"
#include "sensorService.hpp"
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

#ifndef TARGET_MICRO
static constexpr char httpServerIP[] = "http://127.0.0.0:8080/api/hello";
#else
static constexpr char httpServerIP[] = "http://192.168.1.100:8080/api/hello";
#endif

static constexpr uint8_t CONFIG_BUFF_SIZE = 96;

////////////////////////////////////////////////////////////////////////
//					       Private variables
////////////////////////////////////////////////////////////////////////

/**
 * @brief Flag to signal the execution of the measurement task.
 * @details This flag is set by a timer (or simulated SysTick) and cleared after the task runs.
 */
static volatile uint8_t runMeasurementTask = 1;
/**
 * @brief Period for the measurement task in milliseconds.
 */
static uint32_t measurementTaskPeriod = utilities::MS_IN_ONE_MINUTE;

struct hardwareTimeouts			 taskMeasurementControl{&measurementTaskPeriod, &runMeasurementTask};
std::array<hardwareTimeouts*, 2> taskControlContainer{&taskMeasurementControl, nullptr};

////////////////////////////////////////////////////////////////////////
//							    Classes
////////////////////////////////////////////////////////////////////////

virtualRTC	 rtc;
ADC::ADS1115 loggerADC;
/** @brief Manager for processing data from various sensors. */

// sensor::davisPluviometer loggerPluviometer;
// sensor::anemometerDavis	 loggerAnemometer;
// sensor::windVaneDavis	 loggerWindVane(loggerADC);
sensor::thermometer::AHT21 loggerThermometerHygrometer;
// clang-format off
processingManager myProcessingManager(rtc, 
								   	  loggerThermometerHygrometer, 
								      loggerThermometerHygrometer);
sensorService loggerSensorService(loggerADC,
								  loggerThermometerHygrometer,
								  loggerThermometerHygrometer);

// clang-format on
/** @brief Terminal state machine for user configuration via serial interface. */
terminalStateMachine terminalOutput(rtc, loggerSensorService);
/** @brief Component for handling metadata storage on the internal filesystem. */
internalStorageComponent internalStorage;
/** @brief Mediator for the configuration subsystem, connecting terminal and storage. */
configManager loggerConfig(terminalOutput, internalStorage);
/** @brief Manager for logging processed data to files. */
loggerManager myLoggerManager;
/** @brief Manager for network connectivity. */
network::networkManager loggerNetworkManager;
/** @brief HTTP client for sending data to a remote server. */
network::httpClient loggerHttpClient(loggerNetworkManager);

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
	systick::startSystickSimulation(taskControlContainer.data());
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
	uint8_t mac_address[6];

	eth_get_mac_address(mac_address);

	loggerNetworkManager.setMacAddress(mac_address);
	loggerNetworkManager.setIp(loggerMetadataConstants::loggerDefaultIP);
	loggerNetworkManager.setNetmask(loggerMetadataConstants::loggerDefaultNetmask);
	loggerNetworkManager.setGateway(loggerMetadataConstants::loggerDefaultGateway);
#endif
	if (false == loggerNetworkManager.init())
	{
		debug::log<true, debug::logLevel::LOG_ERROR>("Error initializing network manager\r\n");
		while (1);
	}

	/* Configuration */
	measurementTaskPeriod = internalStorage.getMeasurementPeriod();

	terminalOutput.init(terminalState::initState);
	terminalOutput.handler(terminalSignal::ENTRY, nullptr);

	myProcessingManager.init();
	myProcessingManager.setObserver(&myLoggerManager);
	myProcessingManager.setObserver(&loggerHttpClient);

	myLoggerManager.init();

	loggerHttpClient.setURL(httpServerIP);
	loggerHttpClient.setMailBox(myProcessingManager.getSensorInfoBuff());

	loggerADC.init();

	/* Super loop | TODO RTOS */
	while (1)
	{
		configurationTask();
		measurementTask();
		loggerTask();
		networkTask();
	}

#ifndef TARGET_MICRO
	debug::log<true, debug::logLevel::LOG_ERROR>("APP: releasing resources\r\n");
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
	terminalSignal signal = terminalSignal::NONE;
	char		   configBuff[CONFIG_BUFF_SIZE];
	char*		   pPayload = nullptr;

	// Check if data is available in serial port
	bool process_input = serialHandler();
	// Passing signals to handler
	if (process_input)
		signal = getTerminalSignal();

	if (signal == terminalSignal::NONE)
	{
		// Check for SM internal signal
		signal = terminalOutput.getSignal();
	}

	// If data is available, process it and generate signals to terminal SM
	if (process_input || signal == terminalSignal::streamData)
	{
		if (signal == terminalSignal::NONE)
		{
			return;
		}

		pPayload = getSerialBuffer(configBuff, CONFIG_BUFF_SIZE);
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
	if (1 == runMeasurementTask)
	{
		debug::log<true, debug::logLevel::LOG_ALL>("Running measurement task\r\n");

		myProcessingManager.takeMeasurements();
		myProcessingManager.formatData();
		myProcessingManager.notifyObservers();

		runMeasurementTask = 0;
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
	static bool firstCall = true;

	if (true == loggerHttpClient.runTaskFlag())
	{
		if (true == firstCall)
		{
			debug::log<true, debug::logLevel::LOG_ALL>("Running network task\r\n");
			firstCall = false;
		}

		auto postResult = loggerHttpClient.postSensorData();
		if (postResult.has_value())
		{
			if (false == postResult.value())
			{
				debug::log<true, debug::logLevel::LOG_WARNING>("Unable to send data to server\r\n");
			}
			else
			{
				debug::log<true, debug::logLevel::LOG_ALL>("Data sent to server\r\n");
			}

			loggerHttpClient.setRunTaskFlag(false);
		}
	}
	else
	{
		firstCall = true;
	}
}
