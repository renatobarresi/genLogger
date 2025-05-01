#include "logger_manager.hpp"
#include <cstring>

void loggerManager::update(bool infoFlag, char &sensorInformation)
{
    this->_availableData = infoFlag;
    std::strcpy(_sensorData, &sensorInformation);
}