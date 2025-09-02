/**
 * @file main.cpp
 * @author Renato Barresi (renatobarresi@gmail.com)
 * @brief genLogger main application source file
 *
 * genLogger (generic datalogger) is a datalogger
 *
 * The application has five main subsystems:
 *	1) Configuration subsystem
 *	2) Sensor management subsystem
 *  3) Processing subsystem
 *	4) Storage susbsystem
 *	5) Network subsystem
 *
 * The macro TARGET_MICRO must be passed as a compiler definition if
 * the build type is for the target microcontroller. When its defined,
 * components related to the target board are included.
 *
 * @version 0.1
 * @date 2025-01-12
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "main.h"
#include "config_manager.hpp"
#include "internalStorage_component.hpp"
#include "logger_manager.hpp"
#include "processing_manager.hpp"
#include "serialHandler.hpp"
#include "terminal_component.hpp"
#include "virtualRTC.hpp"
#include "virtualTimer.hpp"
#include <new>

// Includes related to the used board
#ifdef TARGET_MICRO
#include "init.h"

void* operator new(std::size_t count) = delete;               // Make sure no library that uses the heap is being used 
#endif

virtualRTC															 rtc;
terminalStateMachine												 terminalOutput(&rtc);
internalStorageComponent											 internalStorage;
configManager														 loggerConfig(&terminalOutput, &internalStorage);
processingManager<sensor::davisPluviometer, sensor::anemometerDavis> myProcessingManager(rtc);
loggerManager														 myLoggerManager;

bool flagKey_F 	   = false;
bool flagKey_I	   = false;
bool flagKey_C	   = false;
bool flagKey_B	   = false;
bool flagKey_N	   = false;
bool flagKey_T	   = false;
bool flagKey_M 	   = false;
bool flagKey_S	   = false;
bool flagKey_Enter = false;

static bool		runMeasurementTask	  = true;
static uint32_t measurementTaskPeriod = 10000;

static char configBuff[96];

// TODO: Add RTOS
static void configurationTask();
static void loggerTask();
static void measurementTask();

#ifndef TARGET_MICRO

void myTickHandler()
{
	static uint64_t tickCount = 0;
	tickCount++;
	if ((tickCount % measurementTaskPeriod) == 0)
	{
		runMeasurementTask = true;
	}
}
#else

struct hardwareTimeouts taskMeasurementControl{&measurementTaskPeriod, 0};

std::array<hardwareTimeouts*, 2> taskControlContainer{&taskMeasurementControl, nullptr};

#endif

/**
 * 
 */
int main()
{
#ifdef TARGET_MICRO
	stm32f429_init(taskControlContainer.data());
#else
	systick::startSystickSimulation(myTickHandler);
#endif

	if (false == internalStorage.initFS())
	{
		while (1);
	}

	if (false == serialHandlerInit())
	{
		while (1);
	}

	measurementTaskPeriod = internalStorage.getMeasurementPeriod();

	terminalOutput.init(terminalState::initState);
	terminalOutput.handler(terminalSignal::ENTRY, nullptr);

	myProcessingManager.init();
	myProcessingManager.setObserver(&myLoggerManager);

	myLoggerManager.init();

	while (1)
	{
		configurationTask();
		measurementTask();
		loggerTask();
	}

#ifndef TARGET_MICRO
	systick::stopSystickSimulation();
#endif

	return 0;
}


void configurationTask()
{
	// Check if data is available in serial port
	bool process_input = serialHandler();

	// If data is available, process it and generate signals to terminal SM
	if (process_input)
	{
		processSerialBuffer();

		if (flagKey_Enter == true)
		{
			getSerialBuffer(configBuff, 96);
		}

		clearSerialBuffer();
	}

	// Passing signals to handler
	if (flagKey_I == true)
	{
		terminalOutput.handler(terminalSignal::pressedKey_I, nullptr);
		flagKey_I = false;
	}
	else if (flagKey_M == true)
	{
		terminalOutput.handler(terminalSignal::pressedKey_M, nullptr);
		flagKey_M = false;
	}
	else if (flagKey_C == true)
	{
		terminalOutput.handler(terminalSignal::pressedKey_C, nullptr);
		flagKey_C = false;
	}
	else if (flagKey_B == true)
	{
		terminalOutput.handler(terminalSignal::pressedKey_B, nullptr);
		flagKey_B = false;
	}
	else if (flagKey_F == true)
	{
		terminalOutput.handler(terminalSignal::pressedKey_F, nullptr);
		flagKey_F = false;
	}
	else if (flagKey_N == true)
	{
		terminalOutput.handler(terminalSignal::pressedKey_N, nullptr);
		flagKey_N = false;
	}
	else if (flagKey_T == true)
	{
		terminalOutput.handler(terminalSignal::pressedKey_T, nullptr);
		flagKey_T = false;
	}
	else if (flagKey_S == true)
	{
		terminalOutput.handler(terminalSignal::pressedKey_S, nullptr);
		flagKey_S = false;
	}
	else if (flagKey_Enter == true)
	{
		terminalOutput.handler(terminalSignal::pressedKey_Enter, configBuff);
		flagKey_Enter = false;
	}

	// If changes in metadata, parse them
	if (internalStorage.getMetadataUpdatedFlag() == true)
	{
		//TODO all metadata relevant parameters
		measurementTaskPeriod = internalStorage.getMeasurementPeriod();
	}
}

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

void loggerTask()
{
	if (true == myLoggerManager.getAvailableDataFlag())
	{
		myLoggerManager.handler();
	}
}
