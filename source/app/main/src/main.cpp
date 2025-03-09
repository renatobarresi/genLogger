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

#ifndef TARGET_MICRO
#include <csignal>
#include <iostream>
#include <unistd.h>

bool flagKey_C = false;

void signalHandler(int signum)
{
	std::cout << "Interrupt signal (" << signum << ") received. \n";

	if (signum == 10) flagKey_C == true;
}
#endif

int main()
{
#ifdef TARGET_MICRO
	stm32f429_init();
#else
	std::cout << "ID: " << getpid() << std::endl;
#endif

	std::signal(SIGUSR1, signalHandler);

	if (false == storage.initFS())
	{
		while (1);
	}

	// storage.storeMetadata();

	terminalOutput.init(terminalState::basicDeviceInfo);

	while (1)
	{
		// Detect if any signlas are available
		terminalOutput.handler();
	}

	return 0;
} 