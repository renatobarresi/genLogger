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
	void update() override;

	/**
     * @brief 
     * 
     */
	void handler();

	bool getAvailableDataFlag();

	void setMailBox(const char* pDataBuff);

  private:
	void _fileManagement(uint8_t confNum);

#ifdef TARGET_MICRO
	std::array<char, 256> _fileName{'\0'};
#endif
	uint16_t			   _infoDataLen;
	struct loggerMetadata* _metadata;
	bool				   _availableData = false;
	const char*			   _pPath		  = nullptr;

	const char* _pDataBuff = nullptr; /// Pointer to the buffer that has the sensors measurements and time measurements were taken
};