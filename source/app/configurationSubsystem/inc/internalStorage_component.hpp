/**
 * @file internalStorage_component.hpp
 * @author Renato Barresi (renatobarresi@gmail.com)
 * @brief Component handling storage and retrieval of internal device data 
 * 
 * All major information about the logger is stored in a file in the internal memory, moreover
 * this stored information is accessed via a pointer to a global variable @ref metadata
 * that is defined in @ref loggerMetadata.cpp
 * 
 * @version 0.1
 * @date 2025-01-24
 *
 * @copyright Copyright (c) 2025
 *
 * @todo Maybe theres a better way to handle the metadata besides a global variable? 
 */

#pragma once

////////////////////////////////////////////////////////////////////////
//							    Includes
////////////////////////////////////////////////////////////////////////

#include "config_mediator.hpp"
#include "loggerMetadata.hpp"
#include <cstdint>
#include <span>

////////////////////////////////////////////////////////////////////////
//							Class definition
////////////////////////////////////////////////////////////////////////

/**
 * @brief This class/component is responsible for handling filesystem operations and metadata storage for the logger
 *
 */
class internalStorageComponent : public configComponent
{
  public:
	/**
   	* @brief Construct a new internal Storage Component object
   	* 
   	*/
	internalStorageComponent();

	/**
	 * @brief initializes the internal filesystem
	 * 
	 * This method gets the pointer to the @ref metadata structure and 
	 * mounts the filesystem (if target is a microcontroller)
	 *
	 * @return true if initialization was successful
	 * @return false if initialization failed
	 */
	bool initFS();

	/**
	 * @brief updates the @ref metadata structure
	 * 
	 * This method uses the filesystem to read the metadata from the 
	 * internal storage and updates the said structure with the stored values
	 * 
	 * @return true if update was successful
	 * @return false if update failed
	 */
	bool retrieveMetadata();

	/**
	 * @brief stores the contents of @ref metadata structure into the internal storage
	 * 
	 * This method opens the metadata.txt file and writes 
	 * the contents of the metadata structure
	 *
	 * @return true 
	 * @return false 
	 */
	bool storeMetadata(const std::span<const char> buff);

	/**
	 * 
	 */
	uint32_t getMeasurementPeriod();

	/**
	 * @brief 
	 */
	bool getMetadataUpdatedFlag();

  private:
	/**
   	* @brief pointer to the global @ref metadata structure
   	* 
   	*/
	mutable struct loggerMetadata* _metadata;

	const char	_defaultPath[13]	 = "metadata.txt";
	const char* _pPath				 = nullptr;
	bool		_metadataUpdatedFlag = false;
	char		_metadataBuffer[METADATA_BUFFER_SIZE];
	bool		_fileSystemInit = false;

	bool _parseMetadataBuffer(char* buffer, loggerMetadata* metadata);
};