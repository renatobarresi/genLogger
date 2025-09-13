#include "logger_manager.hpp"
#include "filesystemWrapper.hpp"
#include "loggerMetadata.hpp"
#include <cstdio>
#include <cstring>
#include <iostream>

#ifndef TARGET_MICRO
#include "utilities.hpp"
#include <string>
#endif

/**
 * @brief The logger manager uses a file system to manipulate files and store/retrieve weather information from an external storage
 */
#ifdef TARGET_MICRO
// Use microcontroller-specific file system
fileSysWrapper fsHandler(2);
#else
fileSysWrapper fsHandler(0);
std::string	   testFolderPath_External = "";
#endif

void loggerManager::update(bool infoFlag)
{
	this->_availableData = infoFlag;
}

bool loggerManager::init()
{
	// Check configuration
	_metadata = getLoggerMetadata();

#ifdef TARGET_MICRO
	std::strncpy(this->_pPath, _metadata->loggerName, std::strlen(_metadata->loggerName));

	if (fsHandler.mount() != true)
	{
		return false;
	}
#else
	testFolderPath_External = utilities::getPathMetadata(_metadata->loggerName);
	this->_pPath			= testFolderPath_External.c_str();
#endif

	return true;
}

void loggerManager::handler()
{
	switch (_metadata->fileCreationPeriod)
	{
		case loggerMetadataConstants::CREATE_ONLY_ONE_FILE:
		{
			// Append data
			if (true == fsHandler.open(this->_pPath, 2))
			{
				fsHandler.write(this->_pDataBuff, std::strlen(this->_pDataBuff));
				fsHandler.close();
			}
			else
			{
				// If opening for append fails, the file might not exist. Try creating it.
				if (true == fsHandler.open(this->_pPath, 1))
				{
					fsHandler.write(this->_pDataBuff, std::strlen(this->_pDataBuff));
					fsHandler.close();
				}
				else
				{
					std::cout << "File could not be opened or created: " << this->_pPath << std::endl;
				}
			}
		}

		case loggerMetadataConstants::CREATE_FILE_A_DAY:
		{
			// todo
			// Check if new week
		}
		default:
		{
		}
		break;
	}
}

bool loggerManager::getAvailableDataFlag()
{
	bool retVal			 = this->_availableData;
	this->_availableData = false;

	return retVal;
}

void loggerManager::setMailBox(const char* pDataBuff)
{
	this->_pDataBuff = pDataBuff;
}