#include "serialHandler.hpp"
#include <cstring>

// For Interrupt simulation in host
#ifndef TARGET_MICRO

#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

std::atomic<bool>		new_data_available(false);
std::atomic<bool>		running(true);
std::string				serialBuffer;
std::mutex				buffer_mutex;
std::condition_variable cv;
bool					input_ready = false;

// Declare the flag variables as external
extern bool flagKey_I;
extern bool flagKey_C;
extern bool flagKey_B;
extern bool flagKey_N;
extern bool flagKey_T;
extern bool flagKey_S;
extern bool flagKey_Enter;

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
#endif

bool serialHandlerInit(void)
{
#ifndef TARGET_MICRO
	// To simulate serial input via DMA
	std::thread input_thread(uartSimulatorThread);
	input_thread.detach(); // Detach the thread so it runs independently
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

#endif
	// todo

	return process_input;
}

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
	else if (serialBuffer.find("\0") != std::string::npos)
	{
		flagKey_Enter = true;
	}
#endif
}

void getSerialBuffer(char* buffer, size_t bufferSize)
{
#ifndef TARGET_MICRO
	std::lock_guard<std::mutex> lock(buffer_mutex);
	std::strncpy(buffer, serialBuffer.c_str(), bufferSize - 1);
	buffer[bufferSize - 1] = '\0'; // Ensure null termination
#endif
}
