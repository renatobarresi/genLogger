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
#include "serialHandler.hpp"
#include "terminal_component.hpp"
#include "virtualRTC.hpp"

virtualRTC				 rtc;
terminalStateMachine	 terminalOutput(&rtc);
internalStorageComponent storage;
configManager			 loggerConfig(&terminalOutput, &storage);

bool flagKey_I	   = false;
bool flagKey_C	   = false;
bool flagKey_B	   = false;
bool flagKey_N	   = false;
bool flagKey_T	   = false;
bool flagKey_S	   = false;
bool flagKey_Enter = false;

int main()
{
#ifdef TARGET_MICRO
	stm32f429_init();
#else
#endif

	if (false == storage.initFS())
	{
		while (1);
	}

	if (false == serialHandlerInit())
	{
		while (1);
	}

	terminalOutput.init(terminalState::initState);

	terminalOutput.handler(terminalSignal::ENTRY, nullptr);

	while (1)
	{
		bool process_input = serialHandler();

		// Process the input outside the lock
		if (process_input)
		{
			processSerialBuffer();
		}

		// Passing signals to handler
		if (flagKey_I == true)
		{
			terminalOutput.handler(terminalSignal::pressedKey_I, nullptr);
			flagKey_I = false;
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
			terminalOutput.handler(terminalSignal::pressedKey_Enter, nullptr);
			flagKey_Enter = false;
		}
	}

	return 0;
}