#include "configuration_subsystem.hpp"
#ifdef TARGET_MICRO
#include "uart.h"
#endif
#include <iostream>
#include <streambuf>

#ifdef TARGET_MICRO
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

void configManager::init()
{
#ifdef TARGET_MICRO
	// Redirect std::cout to UART
	std::cout.rdbuf(&uartStreamBuf);
#endif

	print_banner();
}

void configManager::print_banner()
{
	std::cout << "#############################\r\n";
	std::cout << "genLogger develop version 0.1\r\n";
	std::cout << "#############################\r\n";
}