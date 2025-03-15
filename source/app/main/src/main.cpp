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
#include "terminal_component.hpp"
#include "virtualRTC.hpp"

// Includes related to the used board
#ifdef TARGET_MICRO
#include "init.h"
#endif

virtualRTC				 rtc;
terminalStateMachine	 terminalOutput(&rtc);
internalStorageComponent storage;

configManager loggerConfig(&terminalOutput, &storage);

/**
 * @brief This simulates a microcontroller interrupt 
 * 
 */
#ifndef TARGET_MICRO
#include <csignal>
#include <iostream>
#include <unistd.h>

// Global flags
bool		  flagKey_I = false;
volatile bool flagKey_C = false;
bool		  flagKey_B = false;
bool		  flagKey_N = false;
bool		  flagKey_T = false;
bool		  flagKey_S = false;

void signalHandler(int signum)
{
	std::cout << "Interrupt signal (" << signum << ") received. \n";

	if (signum == 10)
		flagKey_I = true;
	else if (signum == 12)
		flagKey_B = true;
	else if (signum == 34)
		flagKey_C = true;
	else if (signum == 35)
		flagKey_N = true;
	else if (signum == 36)
		flagKey_T = true;
	else if (signum == 37)
		flagKey_S = true;
}
#endif

int main()
{
#ifdef TARGET_MICRO
	stm32f429_init();
#else
	std::cout << "Process ID: " << getpid() << std::endl;
	std::signal(SIGUSR1, signalHandler);
	std::signal(SIGUSR2, signalHandler);
	std::signal(SIGRTMIN, signalHandler);
	std::signal(SIGRTMIN + 1, signalHandler);
	std::signal(SIGRTMIN + 2, signalHandler);
	std::signal(SIGRTMIN + 3, signalHandler);
#endif

	bool initFlag = true;

	if (false == storage.initFS())
	{
		while (1);
	}

	// storage.storeMetadata();

	terminalOutput.init(terminalState::initState);

	while (1)
	{
		// Detect if any signlas are available
		if (initFlag == true)
		{
			terminalOutput.handler(terminalSignal::ENTRY, nullptr);
			initFlag = false;
		}
		if (flagKey_I == true)
		{
			terminalOutput.handler(terminalSignal::pressedKey_I, nullptr);
			flagKey_I = false;
		}
		if (flagKey_C == true)
		{
			terminalOutput.handler(terminalSignal::pressedKey_C, nullptr);
			flagKey_C = false;
		}
		if (flagKey_B == true)
		{
			terminalOutput.handler(terminalSignal::pressedKey_B, nullptr);
			flagKey_B = false;
		}
		if (flagKey_N == true)
		{
			terminalOutput.handler(terminalSignal::pressedKey_N, nullptr);
			flagKey_N = false;
		}
		if (flagKey_T == true)
		{
			terminalOutput.handler(terminalSignal::pressedKey_T, nullptr);
			flagKey_T = false;
		}
		if (flagKey_S == true)
		{
			terminalOutput.handler(terminalSignal::pressedKey_S, nullptr);
			flagKey_S = false;
		}
	}

	return 0;
}