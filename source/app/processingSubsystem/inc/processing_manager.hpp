#pragma once

#include "virtualRTC.hpp"
#include <cstdint>
#include <cstring>
#include "sensorSimulatorConsumer.hpp"

const uint8_t MAX_NUM_OBSERVERS = 10;

class observerInterface
{
  public:
	virtual void update(bool infoFlag, const char*& sensorInformation, const char*& timestamp) = 0;
	virtual ~observerInterface()															   = default;
};

class processingManager
{
  public:
	uint8_t activeObservers = 0;
	char	sensorInfoBuff[1024];

	observerInterface* listOfObservers[MAX_NUM_OBSERVERS];

	processingManager(virtualRTC& rtc) : _loggerRTC(rtc) {}

	void init(void)
	{
#ifndef TARGET_MICRO
		sensorSimulator::init();
#endif
	}

	void setObserver(observerInterface* ref)
	{
		listOfObservers[activeObservers] = ref;

		activeObservers++;
	}

	void processData()
	{
		char timestamp[_loggerRTC.timeStampBuffSize];

		_loggerRTC.getTimestamp(timestamp);

		notify(sensorInfoBuff, timestamp);
	}

  private:
	virtualRTC& _loggerRTC;

	/**
     * @brief iterates through array of listeners 
     * 
     * @param pStr 
     */
	void notify(const char* pStr, const char* timestamp)
	{
		for (uint8_t i = 0; i < activeObservers; i++)
		{
			listOfObservers[i]->update(true, pStr, timestamp);
		}
	}
};