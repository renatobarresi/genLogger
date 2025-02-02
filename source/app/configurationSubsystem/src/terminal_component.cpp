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

void terminalStateMachine::updateLoggerMetadata()
{
	this->configManagerInterface_->notify(this, mediatorEvents::UPDATE_METADATA, nullptr);
}

////////////////////////////////////////////////////////////////////////
//				      Private function implementation
////////////////////////////////////////////////////////////////////////

void printBanner()
{
	std::cout << "#############################\r\n";
	std::cout << "genLogger develop version 0.1\r\n";
	std::cout << "#############################\r\n";
}

void printLoggerMetadata()
{
	struct loggerMetadata* metadata = getLoggerMetadata();

	std::cout << "#############################\r\n";
	std::cout << "Device name: ";

	for (uint16_t i = 0; i < sizeof(metadata->loggerName); i++)
	{
		if (metadata->loggerName[i] == '\0')
			break;
		std::cout << metadata->loggerName[i];
	}

	std::cout << "\r\n#############################\r\n";
}