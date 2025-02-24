/**
 * @file terminal_component.cpp
 * @author Renato Barresi (renatobarresi@gmail.com)
 * @brief
 * @version 0.1
 * @date 2025-01-13
 *
 * @copyright Copyright (c) 2025
 *
 */

////////////////////////////////////////////////////////////////////////
//							    Includes
////////////////////////////////////////////////////////////////////////

#include "terminal_component.hpp"
#include "device_version.hpp"
#include "loggerMetadata.hpp"
#include <iostream>
#include <streambuf>

////////////////////////////////////////////////////////////////////////
//				      Private function prototypes
////////////////////////////////////////////////////////////////////////

void printHelp();

////////////////////////////////////////////////////////////////////////
//						   Stream redirection
////////////////////////////////////////////////////////////////////////

#ifdef TARGET_MICRO
#include "uart.h"

class UartStreamBuf : public std::streambuf
{
  protected:
	int overflow(int c) override
	{
		if (c != EOF)
		{
			uart_tx(static_cast<char>(c), 1);
		}
		return c;
	}
};

UartStreamBuf uartStreamBuf;
std::ostream  uartOut(&uartStreamBuf);
#endif

////////////////////////////////////////////////////////////////////////
//					   Public methods implementation
////////////////////////////////////////////////////////////////////////

void terminalStateMachine::init(terminalState state)
{
#ifdef TARGET_MICRO
	// Redirect std::cout to UART
	std::cout.rdbuf(&uartStreamBuf);
#endif

	this->activeState = state;

	printBanner();

	printHelp();
}

/**
 * @brief the handler checks for pending signals and sends
 * them to the signal dispacher.
 * 
 */
void terminalStateMachine::handler()
{
	terminalState currentState = this->activeState;
	terminalState nextState;
	terminalEvent event = terminalEvent::EVENT_IGNORED;

	// TODO
	// Define how to signal handler that there are pending signals
	uint8_t availableSignal = 0;

	if (1 == availableSignal)
	{
		event = signalDispacher(currentState, terminalSignal::ENTRY);
	}

	if (terminalEvent::EVENT_TRANSITION == event)
	{
		// Exit current state
		signalDispacher(currentState, terminalSignal::EXIT);

		// Enter next state
		nextState = this->activeState;
		signalDispacher(nextState, terminalSignal::ENTRY);
	}
}

////////////////////////////////////////////////////////////////////////
//					 Private methods implementation
////////////////////////////////////////////////////////////////////////

terminalEvent terminalStateMachine::signalDispacher(terminalState state, terminalSignal sig)
{
	terminalEvent event = terminalEvent::EVENT_IGNORED;

	switch (state)
	{
		case terminalState::basicDeviceInfo: {
			switch (sig)
			{
				case terminalSignal::ENTRY: {
					updateLoggerMetadata();

					printLoggerMetadata();

					event = terminalEvent::EVENT_HANDLED;
				}
				break;
				case terminalSignal::EXIT: {
					// Clean Terminal
				}
				break;
				case terminalSignal::pressedKey_C: {
					this->activeState = terminalState::basicDeviceConfig;
					event			  = terminalEvent::EVENT_TRANSITION;
				}
				break;
			}
		}
		break;
		case terminalState::basicDeviceConfig: {
		}
		break;
		default: {
			// do nothing
		}
		break;
	}

	return event;
}

bool terminalStateMachine::updateLoggerMetadata()
{
	uint8_t res = this->configManagerInterface_->notify(this, mediatorEvents::UPDATE_METADATA, nullptr);

	if (res != 1)
	{
		return false;
	}

	return true;
}

void terminalStateMachine::printBanner()
{
	std::cout << "#############################\r\n";
	std::cout << "genLogger version: ";
	std::cout << MAJOR << "." << MINOR << "." << PATCH << "." << DEVELOPMENT << "\r\n";
	std::cout << "#############################\r\n";
}

void terminalStateMachine::printLoggerMetadata()
{
	struct loggerMetadata* metadata = getLoggerMetadata();

	_terminalRTC->getTime(&_timeBuff[0], sizeof(_timeBuff));

	std::cout << "#############################\r\n";
	std::cout << "Device name: " << metadata->loggerName << "\r\n";
	std::cout << "Device time: " << this->_timeBuff << "\r\n";
	std::cout << "#############################\r\n";
}

////////////////////////////////////////////////////////////////////////
//				      Private function implementation
////////////////////////////////////////////////////////////////////////

void printHelp()
{
	std::cout << "#############################\r\n";
	std::cout << "Help:\r\n";
	std::cout << "B - Print Device Info\r\n";
	std::cout << "#############################\r\n";
}