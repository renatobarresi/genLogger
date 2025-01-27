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

internalStorageComponent::internalStorageComponent()
{
	this->thisMetadata = getLoggerMetadata();
}

int8_t internalStorageComponent::retrieveMetadata()
{
	bool status;
	struct loggerMetadata* metadata;

#ifdef TARGET_MICRO
	// Use microcontroller-specific file system
	// fileSysWrapper fileSystem(MICRO_FS_IDENTIFIER);
#else
	fileSysWrapper fileSystem(0); // Use the non-microcontroller implementation
	char simulationFile[] = "/home/renato/renato/CESE_fiuba/proyecto_final/genLogger/firmware/test/"
							"simulationFiles/metadata.txt";
#endif

	// Open the metadata file
	status = fileSystem.open(simulationFile, 0);
	if(status != true)
	{
		// Failed to open the file
		return status;
	}

	// Get the metadata pointer
	metadata = getLoggerMetadata();

	// Read the name from the file into the metadata struct
	char buffer[loggerNameLenght] = {0};
	size_t bytesRead =
		fileSystem.read(buffer, sizeof(buffer) - 1); // Leave space for null terminator
	if(bytesRead > 0)
	{
		// Copy the name from the buffer into the metadata struct
		strncpy(metadata->loggerName, buffer, loggerNameLenght - 1);
		metadata->loggerName[loggerNameLenght - 1] = '\0'; // Ensure null termination
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

void internalStorageComponent::storeMetadata()
{
	//
}