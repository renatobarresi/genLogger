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
#include <cstdlib>
#include <cstring>
#include <span>

////////////////////////////////////////////////////////////////////////
//				      Defines
////////////////////////////////////////////////////////////////////////

constexpr uint16_t MS_IN_MINUTE = 60000;

////////////////////////////////////////////////////////////////////////
//				      Private variables
////////////////////////////////////////////////////////////////////////

#ifdef TARGET_MICRO
// Use microcontroller-specific file system
fileSysWrapper		   fileSystem(1);
static constexpr char* metadataPath = "metadata.txt";
#else
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unistd.h>
fileSysWrapper fileSystem(0); // Use the non-microcontroller implementation
std::string	   testFolderPath = "";
std::string	   getPathMetadata();
#endif

static constexpr char defaultLoggerName[] = "defaultLogger";
static constexpr char defaultMetadata[]	  = "defaultLogger;1;1;1;1";

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
	this->_metadata = getLoggerMetadata();

	retVal = fileSystem.mount();

	if (retVal == true)
	{
#ifndef TARGET_MICRO
		testFolderPath = getPathMetadata();
		this->_pPath   = testFolderPath.c_str();
#else
		this->_pPath = this->_defaultPath;
#endif
		this->_fileSystemInit = true;

		retVal = retrieveMetadata();
	}

	return retVal;
}

// Description in header file //
bool internalStorageComponent::retrieveMetadata()
{
	appAssert(this->_fileSystemInit == true);

	bool retVal;
	int	 bytesRead;

	retVal = fileSystem.open(this->_pPath, 0);
	if (retVal != true)
	{
		// Unable to open the file, create it with default values
		bool status = fileSystem.open(this->_pPath, 1); // Open for writing
		if (status != true)
		{
			return false; // Failed to create the file
		}

		int bytesWrote = fileSystem.write(defaultMetadata, sizeof(defaultMetadata) - 1);

		if (fileSystem.close() != 0 || bytesWrote <= 0)
		{
			return false; // Failed to write to file or close it
		}

		// Now that the file is created with default data, parse that default data.
		// We need a mutable buffer for strtok.
		std::strncpy(_metadataBuffer, defaultMetadata, METADATA_BUFFER_SIZE);
		_metadataBuffer[METADATA_BUFFER_SIZE - 1] = '\0'; // ensure null termination
		_parseMetadataBuffer(_metadataBuffer, this->_metadata);

		// Notify that metadata was updated with default values
		this->_metadataUpdatedFlag = true;
		return true;
	}

	bytesRead = fileSystem.read(this->_metadataBuffer, METADATA_BUFFER_SIZE);

	if (bytesRead > 0)
	{
		// Fill metadata structure
		_parseMetadataBuffer(this->_metadataBuffer, this->_metadata);
	}
	else
	{
		// Handle read error or empty file
		retVal = false; // Indicate an error
	}

	// Close the file
	fileSystem.close();

	return retVal;
}

// Description in header file //
bool internalStorageComponent::storeMetadata(const std::span<const char> buff)
{
	appAssert(this->_fileSystemInit == true);

	bool status = false;

	status = fileSystem.open(this->_pPath, 1);

	if (status != true)
	{
		// Failed to open the file
		return status;
	}

	int bytesWrote = fileSystem.write(buff.data(), buff.size());

	if (fileSystem.close() == 0 || bytesWrote == 0)
	{
		status = false;
	}

	return status;
}

uint32_t internalStorageComponent::getMeasurementPeriod()
{
	return (this->_metadata->fileTransmissionPeriod) * MS_IN_MINUTE;
}

bool internalStorageComponent::getMetadataUpdatedFlag()
{
	bool retVal				   = this->_metadataUpdatedFlag;
	this->_metadataUpdatedFlag = false;
	return retVal;
}

/**
 * @brief Parses metadata buffer and fills the loggerMetadata structure.
 * @param buffer The input char buffer (must be null-terminated or contain `;` as delimiter).
 * @param metadata Reference to the struct to fill.
 * @return true if parsing was successful, false otherwise.
 */
bool internalStorageComponent::_parseMetadataBuffer(char* buffer, loggerMetadata* metadata)
{
	if (!buffer)
		return false;

	// Tokenize with ';' delimiter
	char* token		 = std::strtok(buffer, ";");
	int	  fieldIndex = 0;

	while (token != nullptr)
	{
		switch (fieldIndex)
		{
			case 0: // loggerName
			{
				size_t tokenLen = std::strlen(token);
				if (tokenLen >= loggerNameLenght)
				{
					tokenLen = loggerNameLenght - 1; // Reserve space for null-terminator
				}
				std::memcpy(metadata->loggerName, token, tokenLen);
				metadata->loggerName[tokenLen] = '\0'; // Null-terminate properly
			}
			break;
			case 1: // fileCreationPeriod
				metadata->fileCreationPeriod = static_cast<uint8_t>(std::strtoul(token, nullptr, 10));
				break;
			case 2: // fileTransmissionPeriod
				metadata->fileTransmissionPeriod = static_cast<uint8_t>(std::strtoul(token, nullptr, 10));
				break;
			case 3: // generalMeasurementPeriod
				metadata->generalMeasurementPeriod = static_cast<uint8_t>(std::strtoul(token, nullptr, 10));
				break;
			case 4: // restRequestPeriod
				metadata->restRequestPeriod = static_cast<uint8_t>(std::strtoul(token, nullptr, 10));
				break;
			default:
				// Ignore extra fields
				break;
		}

		token = std::strtok(nullptr, ";");
		++fieldIndex;
	}

	// Validate: did we get at least 5 fields?
	return fieldIndex >= 5;
}

#ifndef TARGET_MICRO
std::string getPathMetadata()
{
	char	buf[4096];
	ssize_t len = readlink("/proc/self/exe", buf, sizeof(buf) - 1);
	if (len == -1)
	{
		throw std::runtime_error("Unable to get executable path");
	}
	buf[len] = '\0';

	std::filesystem::path execDir = std::filesystem::path(buf).parent_path();

	// Convert to string to manipulate
	std::string execDirStr = execDir.string();
	size_t		pos		   = execDirStr.find("/build");
	if (pos != std::string::npos)
	{
		execDirStr.erase(pos);
	}

	// Assign back to path and append metadata path
	execDir = std::filesystem::path(execDirStr);
	execDir /= "test/simulationFiles/metadata.txt";

	std::cout << "Path to metadata: " << execDir << std::endl;

	return execDir.string();
}
#endif