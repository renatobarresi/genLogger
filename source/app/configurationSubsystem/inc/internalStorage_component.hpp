/**
 * @file internalStorage_component.hpp
 * @author Renato Barresi (renatobarresi@gmail.com)
 * @brief Component handling filesystem operations and metadata storage for the logger
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
	bool storeMetadata(const char* pBuff, uint16_t size);

  private:
	/**
   	* @brief pointer to the global @ref metadata structure
   	* 
   	*/
	struct loggerMetadata* thisMetadata;

	bool _fileSystemInit = false;
};