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

/**
 * @brief
 *
 */
void printBanner();

/**
 * @brief
 *
 */
void printLoggerMetadata();

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

	updateLoggerMetadata();

	printLoggerMetadata();
}

void terminalStateMachine::handler() {}

////////////////////////////////////////////////////////////////////////
//					 Private methods implementation
////////////////////////////////////////////////////////////////////////

terminalEvents terminalStateMachine::eventDispacher(terminalState state)
{
	return terminalEvents::EVENT_IGNORED;
}

int8_t terminalStateMachine::updateLoggerMetadata()
{
	uint8_t res = this->configManagerInterface_->notify(this, mediatorEvents::UPDATE_METADATA, nullptr);

	if (res != 1)
	{
		return -1;
	}

	return 1;
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
	std::cout << "Device time: " << _timeBuff << "\r\n";
	std::cout << "#############################\r\n";
}

////////////////////////////////////////////////////////////////////////
//				      Private function implementation
////////////////////////////////////////////////////////////////////////
