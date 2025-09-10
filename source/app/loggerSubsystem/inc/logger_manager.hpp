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

constexpr uint16_t PROCESS_INFO_BUFF_SIZE = 124;

/**
 * @brief Creation periods of measurement files
 */
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
	void update(bool infoFlag) override;

	/**
     * @brief 
     * 
     */
	void handler();

	bool getAvailableDataFlag();

  private:
	void _fileManagement(uint8_t confNum);

	uint16_t			   _infoDataLen;
	struct loggerMetadata* _metadata;
	bool				   _availableData = false;
};