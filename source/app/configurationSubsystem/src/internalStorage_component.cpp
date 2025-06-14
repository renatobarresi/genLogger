/**
 * @file internalStorage_component.cpp
 * @author Renato Barresi (renatobarresi@gmail.com)
 * @brief Source file for the internal storage component, for a better description go to the header file
 * @version 0.1
 * @date 2025-01-24
 *
 * @copyright Copyright (c) 2025
 *
 */

////////////////////////////////////////////////////////////////////////
//							    Includes
////////////////////////////////////////////////////////////////////////

#include "internalStorage_component.hpp"
#include "errorHandler.hpp"
#include "filesystemWrapper.hpp"
#include "loggerMetadata.hpp"
#include <cstring>

////////////////////////////////////////////////////////////////////////
//				      Private variables
////////////////////////////////////////////////////////////////////////

#ifdef TARGET_MICRO
// Use microcontroller-specific file system
fileSysWrapper fileSystem(1);
char		   simulationFile[] = "metadata.txt";
#else
fileSysWrapper fileSystem(0); // Use the non-microcontroller implementation
char		   simulationFile[] = "../../../test/simulationFiles/metadata.txt";
#endif

static char defaultLoggerName[] = "defaultLogger";

////////////////////////////////////////////////////////////////////////
//					   Public methods implementation
////////////////////////////////////////////////////////////////////////

// Description in header file //
internalStorageComponent::internalStorageComponent() {}

// Description in header file //
bool internalStorageComponent::initFS()
{
	bool retVal = false;
	// Get the metadata pointer
	this->thisMetadata = getLoggerMetadata();

	retVal = fileSystem.mount();

	if (retVal == true)
	{
		this->_fileSystemInit = true;

		retVal = retrieveMetadata();
	}

	return retVal;
}

// Description in header file //
bool internalStorageComponent::retrieveMetadata()
{
	appAssert(this->_fileSystemInit == true);

	bool status;

	// Open the metadata file
	status = fileSystem.open(simulationFile, 0);
	if (status != true)
	{
		// Failed to open the file
		return status;
	}

	// Read the name from the file into the metadata struct
	char buffer[loggerNameLenght] = {0};
	// Leave space for null terminator
	int bytesRead = fileSystem.read(buffer, sizeof(buffer) - 1);

	if (bytesRead > 0)
	{
		// Copy the name from the buffer into the metadata struct
		std::strncpy(this->thisMetadata->loggerName, buffer, loggerNameLenght - 1);
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

// Description in header file //
bool internalStorageComponent::storeMetadata(const char* pBuff, uint16_t size)
{
	appAssert(this->_fileSystemInit == true);

	bool status = false;

	status = fileSystem.open(simulationFile, 1);

	if (status != true)
	{
		// Failed to open the file
		return status;
	}

	int bytesWrote = fileSystem.write(pBuff, size);

	if (fileSystem.close() == 0 || bytesWrote == 0)
	{
		status = false;
	}

	return status;
}