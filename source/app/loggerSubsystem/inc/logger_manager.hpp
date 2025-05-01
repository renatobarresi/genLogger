/**
 * @file logger_manager.hpp
 * @author your name (you@domain.com)
 * @brief 

 loggerManager -> externalStorage_component -> fileSystemWrapper -> virtualSDDriver -> sdDriver -> HAL SPI (?) -> uC SPI

 The logger subsystem is part of an observer pattern, where it listens for events comming from the processing subsystem

 * @version 0.1
 * @date 2025-03-27
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once

#include "processing_manager.hpp"

constexpr uint16_t PROCESS_INFO_BUFF_SIZE = 1024;

class loggerManager : public observerInterface
{
    public: 

    loggerManager(processingManager *processingManager) : _processingManager(processingManager)
    {
    
    }

    void update(bool infoFlag, char &sensorInformation) override;


    private:
    
    processingManager *_processingManager;
    char _sensorData[PROCESS_INFO_BUFF_SIZE];
    bool _availableData;
};