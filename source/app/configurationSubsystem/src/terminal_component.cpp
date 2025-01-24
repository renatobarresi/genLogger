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
#include <iostream>
#include <streambuf>

////////////////////////////////////////////////////////////////////////
//				      Private function prototypes
////////////////////////////////////////////////////////////////////////

void print_banner();

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
		if(c != EOF)
		{
			uart_tx(static_cast<char>(c), 1);
		}
		return c;
	}
};

UartStreamBuf uartStreamBuf;
std::ostream uartOut(&uartStreamBuf);
#endif

////////////////////////////////////////////////////////////////////////
//					   Public methods implementation
////////////////////////////////////////////////////////////////////////
/**
 * @name
 * @brief
 *
 */
void terminalStateMachine::init(terminalState state)
{
#ifdef TARGET_MICRO
	// Redirect std::cout to UART
	std::cout.rdbuf(&uartStreamBuf);
#endif

	this->activeState = state;

	print_banner();
}

////////////////////////////////////////////////////////////////////////
//					   Private methods implementation
////////////////////////////////////////////////////////////////////////

terminalEvents terminalStateMachine::eventDispacher(terminalState state)
{
	return terminalEvents::EVENT_IGNORED;
}

////////////////////////////////////////////////////////////////////////
//				      Private function implementation
////////////////////////////////////////////////////////////////////////

/**
 * @name
 * @brief
 *
 */
void print_banner()
{
	std::cout << "#############################\r\n";
	std::cout << "genLogger develop version 0.1\r\n";
	std::cout << "#############################\r\n";
}