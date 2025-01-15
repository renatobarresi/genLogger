/**
 * @file configuration_subsystem.cpp
 * @author Renato Barresi (renatobarresi@gmail.com)
 * @brief
 * @version 0.1
 * @date 2025-01-13
 *
 * @copyright Copyright (c) 2025
 *
 */

/*###############################################*/
// 					Includes
/*###############################################*/
#include "configuration_subsystem.hpp"
#include <iostream>
#include <streambuf>

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

/*###############################################*/
// 				Methods implementation
/*###############################################*/
/**
 * @name
 * @brief
 *
 */
void configManager::init()
{
#ifdef TARGET_MICRO
	// Redirect std::cout to UART
	std::cout.rdbuf(&uartStreamBuf);
#endif

	print_banner();
}

/**
 * @name
 * @brief
 *
 */
void configManager::print_banner()
{
	std::cout << "#############################\r\n";
	std::cout << "genLogger develop version 0.1\r\n";
	std::cout << "#############################\r\n";
}