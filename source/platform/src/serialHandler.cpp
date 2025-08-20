/**
 * @file serialHandler.cpp
 * @author Renato Barresi (renatobarresi@gmail.com)
 * @brief 
 *
 *	serialHandler -> uart
 *  
 *  serialHandler signals uart to start listening for data (RX DMA). 
 *	uart provides serialHandler with notification that data is available and 
 *  then serialHandler consumes the data.
 *
 * @version 0.1
 * @date 2025-03-23
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "serialHandler.hpp"
#include <cstring>
#include <iostream>

#ifndef TARGET_MICRO

#include <atomic>
#include <condition_variable>

#include <mutex>
#include <string>
#include <thread>
#else
#include "uart.h"
#include <cstring>
#endif

// TODO: this extern global variables are just temporary
// way of communicating between main task and terminal SM
// Declare the flag variables as external
extern bool flagKey_I;
extern bool flagKey_M;
extern bool flagKey_C;
extern bool flagKey_B;
extern bool flagKey_N;
extern bool flagKey_T;
extern bool flagKey_S;
extern bool flagKey_F;
extern bool flagKey_Enter;

// For Interrupt simulation in host
#ifndef TARGET_MICRO

std::atomic<bool>		new_data_available(false);
std::atomic<bool>		running(true);
std::string				serialBuffer;
std::mutex				buffer_mutex;
std::condition_variable cv;
bool					input_ready = false;

// Function for the input thread
void uartSimulatorThread()
{
	while (running)
	{
		std::string temp;
		std::getline(std::cin, temp);

		if (!temp.empty())
		{
			std::lock_guard<std::mutex> lock(buffer_mutex);
			serialBuffer	   = temp;
			input_ready		   = true;
			new_data_available = true;
			cv.notify_one();
		}
	}
}
#else
#define SERIAL_BUFFER_SIZE 1024
char serialBuffer[SERIAL_BUFFER_SIZE];
#endif

bool serialHandlerInit(void)
{
#ifndef TARGET_MICRO
	// To simulate serial input via DMA
	std::thread input_thread(uartSimulatorThread);
	input_thread.detach(); // Detach the thread so it runs independently
#else
	uart_start_rx_dma();
#endif

	return true;
}

bool serialHandler(void)
{
	bool process_input = false;

#ifndef TARGET_MICRO

	std::string current_input;

	// Safely check and get the input
	{
		std::lock_guard<std::mutex> lock(buffer_mutex);
		if (new_data_available)
		{
			current_input = serialBuffer;
			//serialBuffer.clear();
			new_data_available = false;
			process_input	   = true;
		}
	}

#else
	uint8_t auxVar;

	auxVar = uart_isDataAvailable();

	if (1 == auxVar)
	{
		process_input = true;
		uart_start_rx_dma();
		uart_getAvailableData(reinterpret_cast<uint8_t*>(serialBuffer), SERIAL_BUFFER_SIZE);
	}
	else
	{
		process_input = false;
	}
#endif

	return process_input;
}

/**
 * @brief 
 * 
 */
void processSerialBuffer()
{
#ifndef TARGET_MICRO
	if (serialBuffer == "I")
	{
		flagKey_I = true;
	}
	else if (serialBuffer == "C")
	{
		flagKey_C = true;
	}
	else if (serialBuffer == "B")
	{
		flagKey_B = true;
	}
	else if (serialBuffer == "N")
	{
		flagKey_N = true;
	}
	else if (serialBuffer == "T")
	{
		flagKey_T = true;
	}
	else if (serialBuffer == "S")
	{
		flagKey_S = true;
	}
	else if (serialBuffer == "F")
	{
		flagKey_F = true;
	}
	else if (serialBuffer == "M")
	{
		flagKey_M = true;
	}
	else if (serialBuffer.find("\0") != std::string::npos)
	{
		flagKey_Enter = true;
	}
#else
	if (0 == strcmp(serialBuffer, "I"))
	{
		flagKey_I = true;
	}
	else if (0 == strcmp(serialBuffer, "C"))
	{
		flagKey_C = true;
	}
	else if (0 == strcmp(serialBuffer, "B"))
	{
		flagKey_B = true;
	}
	else if (0 == strcmp(serialBuffer, "N"))
	{
		flagKey_N = true;
	}
	else if (0 == strcmp(serialBuffer, "T"))
	{
		flagKey_T = true;
	}
	else if (0 == strcmp(serialBuffer, "S"))
	{
		flagKey_S = true;
	}
	else if (std::strlen(serialBuffer) > 1)
	{
		flagKey_Enter = true;
	}
#endif

	// std::cout << "Pressed key: " << serialBuffer << std::endl;
}

/**
 * @brief Get the Serial Buffer object
 * 
 * @param buffer 
 * @param bufferSize 
 */
void getSerialBuffer(char* buffer, size_t bufferSize)
{
#ifndef TARGET_MICRO
	std::lock_guard<std::mutex> lock(buffer_mutex);
	std::strncpy(buffer, serialBuffer.c_str(), bufferSize - 1);
	buffer[bufferSize - 1] = '\0'; // Ensure null termination
#else
	std::strncpy(buffer, serialBuffer, bufferSize - 1);
	buffer[bufferSize - 1] = '\0'; // Ensure null termination
#endif
}

void clearSerialBuffer()
{
#ifndef TARGET_MICRO
	std::lock_guard<std::mutex> lock(buffer_mutex);
	serialBuffer.clear();
#else
	std::memset(serialBuffer, 0, SERIAL_BUFFER_SIZE);
#endif
}