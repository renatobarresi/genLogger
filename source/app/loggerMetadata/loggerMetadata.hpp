/**
 * @file loggerMetadata.hpp
 * @author Renato Barresi (renatobarresi@gmail.com)
 * @brief Header file for the handling of the internal information about the logger

 The information stored in the device are: 

 Device name
 Device current file transmission period (for ftp servers)
 Device last computed line period (for https servers) 
   (computed line is the text that is generated after all sensors data is aquired and contains all the data)

 * @version 0.1
 * @date 2025-01-24
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

////////////////////////////////////////////////////////////////////////
//							    Includes
////////////////////////////////////////////////////////////////////////

#include <cstdint>

////////////////////////////////////////////////////////////////////////
//							    Constants
////////////////////////////////////////////////////////////////////////

constexpr uint16_t loggerNameLenght = 96;

////////////////////////////////////////////////////////////////////////
//							    Structures
////////////////////////////////////////////////////////////////////////

/**
 * @brief This structure represents the basic metadata for the logger
 * 
 */
struct loggerMetadata
{
	char	loggerName[loggerNameLenght];
	uint8_t fileCreationPeriod = 4;	  // loggerSubsystem has to create files, this parameter indicates how often it does. (for more information, read externalStorage_component.hpp documentation)
	uint8_t fileTransmissionPeriod;	  // Period (minutes) for sending the current file to the server
	uint8_t generalMeasurementPeriod; // Period (seconds) for making a measurement and storing it (not all sensors follow this period)
	uint8_t restRequestPeriod;		  // Period (minutes) for sending last computed data line to the server
};

////////////////////////////////////////////////////////////////////////
//							    Functions
////////////////////////////////////////////////////////////////////////

/**
 * @brief Get the pointer to @ref metadata structure
 * 
 * @return struct loggerMetadata* 
 */
struct loggerMetadata* getLoggerMetadata();