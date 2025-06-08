#include "logger_manager.hpp"
#include "filesystemWrapper.hpp"
#include "loggerMetadata.hpp"
#include <cstdio>
#include <cstring>
#include <iostream>

/**
 * @brief The logger manager uses a file system to manipulate files and store/retrieve weather information from an external storage
 */
#ifdef TARGET_MICRO
// Use microcontroller-specific file system
fileSysWrapper fsHandler(2);
#else
fileSysWrapper fsHandler(0);
char		   simulationFileFolder[] = "../../../test/simulationFiles/externalMemory"; // This folder simulates the root path of the external storage device
#endif

void loggerManager::update(bool infoFlag, const char*& sensorInformation, const char*& timestamp)
{
	this->_availableData = infoFlag;
	std::sprintf(sensorData, "%s;%s", timestamp, sensorInformation);
}

bool loggerManager::init()
{
	// Check configuration
	_metadata = getLoggerMetadata();

#ifdef TARGET_MICRO
	if (fsHandler.mount() != true)
	{
		return false;
	}
#endif

	return true;
}

void loggerManager::handler()
{
	char fileName[56];

	// Check if new information is available
	if (false == _availableData)
	{
		return;
	}

	switch (_metadata->fileCreationPeriod)
	{
		case 0:
		{
			// todo
			// Check if new day
		}

		case 1:
		{
			// todo
			// Check if new week
		}

		case 2:
		{
			// todo
			// Check if new month
		}

		case 3:
		{
			// todo
			// Check if new year
		}

		case 4:
		default:
		{
// Define file name
#ifdef TARGET_MICRO
			//std::strncpy(fileName, _metadata->loggerName, std::strlen(fileName));
			std::sprintf(fileName, "%s.txt", _metadata->loggerName);
#else
			std::sprintf(fileName, "%s/%s", simulationFileFolder, _metadata->loggerName);
#endif
			// Append data
			if (true == fsHandler.open(fileName, 1))
			{
				fsHandler.write(this->sensorData, std::strlen(this->sensorData));

				fsHandler.close();
			}
			else
			{
				std::cout << "File not opened" << std::endl;
			}
		}
		break;
	}
}
