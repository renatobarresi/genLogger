#include "internalStorage_wrapper.hpp"

#ifdef TARGET_MICRO
// Include LittleFS header files
#include "littleFSInterface.h"
#else
#include <fstream>
#include <iostream>
#endif

// File handle object or pointer (abstracted)
#ifdef TARGET_MICRO
// LittleFS::File file;
#else
std::fstream file;
#endif

/**
 * @brief Opens a file for reading or writing.
 *
 * @param fileName The name of the file to open.
 * @param mode The mode in which to open the file (e.g., read or write).
 *             Mode values could be based on LittleFS or custom-defined enums.
 * @return int8_t Returns 0 on success, -1 on failure.
 */
int8_t open(const char* fileName, uint8_t mode)
{
#ifdef TARGET_MICRO
	// Convert mode to LittleFS mode
	/*const char* fsMode = nullptr;
	switch(mode)
	{
		case 0: // Read mode
			fsMode = "r";
			break;
		case 1: // Write mode
			fsMode = "w";
			break;
		case 2: // Append mode
			fsMode = "a";
			break;
		default:
			return -1; // Invalid mode
	}*/

	/*file = LittleFS.open(fileName, fsMode);
	if(!file)
	{
		return -1; // Failed to open file
	}*/
#else
	// Convert mode to std::fstream mode
	std::ios::openmode fsMode = std::ios::in; // Default to read
	switch(mode)
	{
		case 0: // Read mode
			fsMode = std::ios::in;
			break;
		case 1: // Write mode
			fsMode = std::ios::out | std::ios::trunc;
			break;
		case 2: // Append mode
			fsMode = std::ios::out | std::ios::app;
			break;
		default:
			return -1; // Invalid mode
	}

	file.open(fileName, fsMode);
	if(!file.is_open())
	{
		return -1; // Failed to open file
	}
#endif
	return 0; // Success
}

/**
 * @brief Closes the currently open file.
 *
 * @return int8_t Returns 0 on success, -1 on failure.
 */
int8_t close()
{
#ifdef TARGET_MICRO
	/*if(file)
	{
		file.close();
		return 0;
	}*/
	return -1; // No file to close
#else
	if(file.is_open())
	{
		file.close();
		return 0;
	}
	return -1; // No file to close
#endif
}

/**
 * @brief Reads data from the currently open file.
 *
 * @param buffer Pointer to the buffer where data will be stored.
 * @param size Number of bytes to read.
 * @return int32_t Number of bytes read, or -1 on failure.
 */
int32_t read(char* buffer, size_t size)
{
#ifdef TARGET_MICRO
	/*if(file)
	{
		return file.readBytes(buffer, size);
	}*/
	return -1; // No file to read from
#else
	if(file.is_open() && file.read(buffer, size))
	{
		return static_cast<int32_t>(file.gcount());
	}
	return -1; // Read failed
#endif
}

/**
 * @brief Writes data to the currently open file.
 *
 * @param buffer Pointer to the data to be written.
 * @param size Number of bytes to write.
 * @return int32_t Number of bytes written, or -1 on failure.
 */
int32_t write(const char* buffer, size_t size)
{
#ifdef TARGET_MICRO
	/*if(file)
	{
		return file.write((const uint8_t*)buffer, size);
	}*/
	return -1; // No file to write to
#else
	if(file.is_open())
	{
		file.write(buffer, size);
		if(file.fail())
		{
			return -1; // Write failed
		}
		return static_cast<int32_t>(size);
	}
	return -1; // No file to write to
#endif
}
