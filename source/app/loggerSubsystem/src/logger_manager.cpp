#include "logger_manager.hpp"
#include "filesystemWrapper.hpp"
#include "loggerMetadata.hpp"
#include <cstdio>
#include <cstring>
#include <iostream>

#ifdef TARGET_MICRO
// Use microcontroller-specific file system
fileSysWrapper fileSystem(1);
char		   simulationFile[] = "metadata.txt";
#else
fileSysWrapper fsHandler(0); // Use the non-microcontroller implementation
char		   simulationFileFolder[] = "../../../test/simulationFiles/externalMemory";
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
	if (true == fsHandler(3))
#else
	return true;
#endif
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
			std::cout << "1 file for all data" << std::endl;
// Define file name
#ifdef TARGET_TYPE
			std::strncpy(fileName, _metadata->loggerName, std::strlen(fileName));
#else
			std::sprintf(fileName, "%s/%s", simulationFileFolder, _metadata->loggerName);
#endif
			// Append data
			if (true == fsHandler.open(fileName, 2))
			{
				std::cout << "File opened: " << fileName << std::endl;

				fsHandler.write(this->sensorData, this->_infoDataLen);

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
