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

#include "loggerMetadata.hpp"
#include "processing_manager.hpp"

constexpr uint16_t PROCESS_INFO_BUFF_SIZE = 1024;

enum class fileGenerationConf_t
{
	DAILY_FILE,
	WEEKLY_FILE,
	MONTHLY_FILE,
	YEARLY_FILE,
	ONE_FILE
};

class loggerManager : public observerInterface
{
  public:
	/**
     * @brief Construct a new logger Manager object
     * 
     * @param processingManager 
     */
	loggerManager(processingManager* processingManager) : _processingManager(processingManager) {}

	bool init();

	/**
     * @brief 
     * 
     */
	fileGenerationConf_t typeOfFile = fileGenerationConf_t::DAILY_FILE;

	/**
     * @brief 
     * 
     * @param infoFlag 
     * @param sensorInformation 
     */
	void update(bool infoFlag, const char*& sensorInformation, const char*& timestamp) override;

	/**
     * @brief 
     * 
     */
	void handler();

	char sensorData[PROCESS_INFO_BUFF_SIZE];

  private:
	void _fileManagement(uint8_t confNum);

	processingManager*	   _processingManager;
	uint16_t			   _infoDataLen;
	struct loggerMetadata* _metadata;
	bool				   _availableData;
};