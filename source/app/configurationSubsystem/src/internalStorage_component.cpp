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
fileSysWrapper fileSystem(1);
char		   simulationFile[] = "metadata.txt";
#else
fileSysWrapper fileSystem(0); // Use the non-microcontroller implementation
char		   simulationFile[] = "/home/renato/renato/CESE_fiuba/proyecto_final/genLogger/firmware/test/simulationFiles/metadata.txt";
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
	this->_metadata = getLoggerMetadata();

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

	bool retVal;
	int	 bytesRead;

	retVal = fileSystem.open(simulationFile, 0);
	if (retVal != true)
	{
		return retVal;
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

	status = fileSystem.open(simulationFile, 1);

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