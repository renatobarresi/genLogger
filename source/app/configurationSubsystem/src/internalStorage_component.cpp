/**
 * @file internalStorage_component.cpp
 * @author Renato Barresi (renatobarresi@gmail.com)
 * @brief
 * @version 0.1
 * @date 2025-01-24
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "internalStorage_component.hpp"
#include "filesystem_wrapper.hpp"
#include "loggerMetadata.hpp"
#include <cstring>

#ifdef TARGET_MICRO
// Use microcontroller-specific file system
fileSysWrapper fileSystem(1);
char simulationFile[] = "metadata.txt";
#else
fileSysWrapper fileSystem(0); // Use the non-microcontroller implementation
char simulationFile[] = "/home/renato/renato/CESE_fiuba/proyecto_final/genLogger/firmware/test/"
						"simulationFiles/metadata.txt";
#endif

internalStorageComponent::internalStorageComponent() {}

bool internalStorageComponent::initFS()
{
	// Get the metadata pointer
	this->thisMetadata = getLoggerMetadata();

	return fileSystem.mount();
}

bool internalStorageComponent::retrieveMetadata()
{
	bool status;

	// Open the metadata file
	status = fileSystem.open(simulationFile, 0);
	if(status != true)
	{
		// Failed to open the file
		return status;
	}

	// Read the name from the file into the metadata struct
	char buffer[loggerNameLenght] = {0};
	// Leave space for null terminator
	int bytesRead = fileSystem.read(buffer, sizeof(buffer) - 1);

	if(bytesRead > 0)
	{
		// Copy the name from the buffer into the metadata struct
		strncpy(this->thisMetadata->loggerName, buffer, loggerNameLenght - 1);
		this->thisMetadata->loggerName[loggerNameLenght - 1] = '\0'; // Ensure null termination
	}
	else
	{
		// Handle read error or empty file
		status = -1; // Indicate an error
	}

	// Close the file
	fileSystem.close();

	return status;
}

bool internalStorageComponent::storeMetadata()
{
	bool status;
	/*
	strncpy("defaultLogger", this->thisMetadata->loggerName, strlen("defaultLogger"));
	status = fileSystem.open(simulationFile, 1);

	if(status != true)
	{
		// Failed to open the file
		return status;
	}

	int bytesWrote = fileSystem.write(this->thisMetadata->loggerName, strlen("defaultLogger"));

	// Close the file
	fileSystem.close();
*/

	return status;
}