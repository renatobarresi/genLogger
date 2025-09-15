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
#include "utilities.hpp"
#include <cstdlib>
#include <cstring>
#include <span>

#ifndef TARGET_MICRO
#include <filesystem>
#include <stdexcept>
#include <string>
#include <unistd.h>
#endif
////////////////////////////////////////////////////////////////////////
//				      Defines
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
//				      Private variables
////////////////////////////////////////////////////////////////////////

/** @brief Filesystem wrapper instance. The implementation is selected at compile time. */
#ifdef TARGET_MICRO
// Use microcontroller-specific file system
fileSysWrapper fileSystem(1);
/** @brief Path to the metadata file on the target device. */
static constexpr char* metadataPath = "metadata.txt";
#else

/** @brief Filesystem wrapper instance for host-based execution (testing/simulation). */
fileSysWrapper fileSystem(0); // Use the non-microcontroller implementation
/** @brief Path to the folder containing test files on the host. */
std::string testFolderPath = "";
#endif

/** @brief Default name for the logger if not configured. */
static constexpr char defaultLoggerName[] = "defaultLogger";
/** @brief Default metadata string used to create the metadata file if it doesn't exist. */
static constexpr char defaultMetadata[] = "defaultLogger;1;1;1;1";

////////////////////////////////////////////////////////////////////////
//					   Public methods implementation
////////////////////////////////////////////////////////////////////////

/**
 * @brief Construct a new internal Storage Component object.
 *
 */
internalStorageComponent::internalStorageComponent() {}

/**
 * @brief Initializes the internal filesystem and loads metadata.
 * @details This function mounts the filesystem, determines the path for the metadata file,
 * and attempts to retrieve the existing metadata. If retrieval fails (e.g., file not found),
 * it creates a default metadata file.
 * @return true if the filesystem was initialized and metadata was loaded successfully.
 * @return false if there was an error mounting the filesystem or handling the metadata file.
 */
bool internalStorageComponent::initFS()
{
	bool retVal = false;
	// Get the metadata pointer
	this->_metadata = getLoggerMetadata();

	retVal = fileSystem.mount();

	if (retVal == true)
	{
#ifndef TARGET_MICRO
		testFolderPath = utilities::getPathMetadata("metadata.txt");
		this->_pPath   = testFolderPath.c_str();
#else
		this->_pPath = this->_defaultPath;
#endif
		this->_fileSystemInit = true;

		retVal = retrieveMetadata();
	}

	return retVal;
}

/**
 * @brief Retrieves metadata from the internal storage.
 * @details This function reads the metadata file from the filesystem. If the file cannot be
 * opened, it creates a new one with default values. The content of the file is then
 * parsed to populate the `_metadata` structure.
 * @return true if metadata was successfully read and parsed.
 * @return false if the file could not be opened, created, written to, or if the file was
 * empty or could not be read.
 */
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

/**
 * @brief Stores the provided metadata buffer into the internal storage.
 * @details This function opens the metadata file for writing and overwrites its content
 * with the data provided in the buffer.
 * @param buff A `std::span` containing the new metadata to be written.
 * @return true if the metadata was written and the file was closed successfully.
 * @return false if the file could not be opened or if the write operation failed.
 */
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

/**
 * @brief Gets the measurement period from the metadata.
 *
 * @return uint32_t The measurement period in milliseconds.
 */
uint32_t internalStorageComponent::getMeasurementPeriod()
{
	return (this->_metadata->generalMeasurementPeriod) * (utilities::MS_IN_ONE_MINUTE);
}

/**
 * @brief Gets the metadata updated flag and resets it.
 *
 * @return true if the metadata has been updated since the last check, false otherwise.
 */
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
