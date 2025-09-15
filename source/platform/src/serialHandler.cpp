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

#ifndef TARGET_MICRO

#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#else
#include "terminal_component.hpp"
#include "uart.h"
#include <cstring>
#endif

// clang-format off
static terminalKeyMap keyMap[] =
{
	{terminalSignal::pressedKey_I, 'I'},
	{terminalSignal::pressedKey_C, 'C'},
	{terminalSignal::pressedKey_B, 'B'},
	{terminalSignal::pressedKey_N, 'N'},
	{terminalSignal::pressedKey_T, 'T'},
	{terminalSignal::pressedKey_S, 'S'},
	{terminalSignal::pressedKey_F, 'F'},
	{terminalSignal::pressedKey_M, 'M'},
	{terminalSignal::pressedKey_Enter, '\r'},
};
// clang-format on

static bool payload = false;

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
			current_input	   = serialBuffer;
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

terminalSignal getTerminalSignal()
{
	terminalSignal signal = terminalSignal::NONE;

	for (uint8_t index = 0; index < sizeof(keyMap) / sizeof(terminalKeyMap); index++)
	{
#ifndef TARGET_MICRO
		if (serialBuffer.length() < 2)
		{
			if (serialBuffer.c_str()[0] == keyMap[index].key)
			{
#else
		if (strlen(serialBuffer) < 2)
		{
			if (serialBuffer[0] == keyMap[index].key)
			{
#endif
				signal = keyMap[index].signal;
				break;
			}
		}
	}

#ifndef TARGET_MICRO
	if (terminalSignal::NONE == signal && serialBuffer.length() > 0)
#else
	if (terminalSignal::NONE == signal && strlen(serialBuffer) > 0)
#endif
	{
		payload = true;
		signal	= terminalSignal::pressedKey_Enter;
	}

	return signal;
}

/**
 * @brief Get the Serial Buffer object
 * 
 * @param buffer 
 * @param bufferSize 
 */
char* getSerialBuffer(char* buffer, size_t bufferSize)
{
	if (payload == false)
	{
		return nullptr;
	}

#ifndef TARGET_MICRO
	std::lock_guard<std::mutex> lock(buffer_mutex);
	std::strncpy(buffer, serialBuffer.c_str(), bufferSize - 1);
	buffer[bufferSize - 1] = '\0'; // Ensure null termination
#else
	std::strncpy(buffer, serialBuffer, bufferSize - 1);
	buffer[bufferSize - 1] = '\0'; // Ensure null termination
#endif

	payload = false;

	return buffer;
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