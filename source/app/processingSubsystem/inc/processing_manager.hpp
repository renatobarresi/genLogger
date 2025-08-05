#pragma once

#include "pluviometer.hpp"
#include "virtualRTC.hpp"
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstring>

#ifndef TARGET_MICRO
#include "sensorSimulatorConsumer.hpp"
#endif

const uint8_t  MAX_NUM_OBSERVERS   = 10;
const uint16_t MSRD_DATA_BUFF_SIZE = 1024;

class observerInterface
{
  public:
	virtual void update(bool infoFlag, const char*& sensorInformation, const char*& timestamp) = 0;
	virtual ~observerInterface()															   = default;
};

class processingManager
{
  public:
	uint8_t											  activeObservers = 0;				   /// How many components are listening for notifications
	char											  sensorInfoBuff[MSRD_DATA_BUFF_SIZE]; /// This buffer contains the measured information and its consumed by the observers
	std::array<observerInterface*, MAX_NUM_OBSERVERS> listOfObservers;					   /// components that will be notified with processed data. e.g loggerSubsystem, networkSubsystem

	/**
	 * @brief constructor
	 */
	processingManager(virtualRTC& rtc) : _loggerRTC(rtc) {}

	void init(void)
	{
#ifndef TARGET_MICRO
		sensorSimulator::init();
#endif
		loggerPluviometer = sensor::pluviometerFactory::createPluviometer(sensor::pluviometerType::DAVIS);
	}

	void setObserver(observerInterface* ref)
	{
		listOfObservers[activeObservers++] = ref;
	}

	void takeMeasurements()
	{
		_rainInMm = loggerPluviometer->getRain();
		_loggerRTC.getTimestamp(timestamp);
	}

	void processData()
	{
		// Append measurements with time they were taken
		sprintf(sensorInfoBuff, "%s;%d;", timestamp, _rainInMm);
		//strncpy(sensorInfoBuff, timestamp, strlen(timestamp));

		notify(sensorInfoBuff, timestamp);
	}

  private:
	virtualRTC&			 _loggerRTC;
	char				 timestamp[100]; /// Used to store the time and date of when the measurements were made
	sensor::pluviometer* loggerPluviometer;

	uint16_t _rainInMm;

	/**
     * @brief iterates through array of listeners 
     * 
     * @param pStr 
     */
	void notify(const char* pStr, const char* timestamp)
	{
		// no range loop because not all pointers in listOfObservers are valid
		for (uint8_t i = 0; i < activeObservers; i++)
		{
			listOfObservers[i]->update(true, pStr, timestamp);
		}
	}
};