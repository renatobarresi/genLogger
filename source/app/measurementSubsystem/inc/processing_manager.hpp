#pragma once

#include "anemometer.hpp"
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

class observerInterface // change class name, maybe processingMngObserver or IProcessing
{
  public:
	virtual void update(bool infoFlag, const char* sensorInformation, const char* timestamp) = 0;
	virtual ~observerInterface()															 = default;
};

/**
 * @brief 
 * 
 * @tparam TPluviometer 
 * @tparam TAnemometer 
 */
template<typename TPluviometer, typename TAnemometer>
class processingManager
{
  public:
	//char sensorInfoBuff[MSRD_DATA_BUFF_SIZE]; /// This buffer contains the measured information and its consumed by the observers
	// sensorInfoBuff is public and not private to be able to test the notification system, there's gotta be a better way of testing it tho.
	std::array<char, MSRD_DATA_BUFF_SIZE> sensorInfoBuff;
	/**
	 * @brief constructor
	 */
	processingManager(virtualRTC& rtc) : _loggerRTC(rtc) {}

	void init(void)
	{
#ifndef TARGET_MICRO
		sensorSimulator::init();
#endif
	}

	void setObserver(observerInterface* ref)
	{
		_listOfObservers[_activeObservers++] = ref;
	}

	void takeMeasurements()
	{
		_loggerRTC.getTimestamp(_timestamp.data());

		// TODO all sensors
		_rainInMm		= _loggerPluviometer.getRain();
		_windSpeedInMPS = _loggerAnemometer.getWindSpeed();
	}

	void formatData()
	{
		// Append measurements with time they were taken
		sprintf(sensorInfoBuff.data(), "%d;%d;", _rainInMm, _windSpeedInMPS);
	}

	void notifyObservers()
	{
		notify(sensorInfoBuff.data(), _timestamp.data());
	}

  private:
	std::array<observerInterface*, MAX_NUM_OBSERVERS> _listOfObservers;		/// components that will be notified with processed data. e.g loggerSubsystem, networkSubsystem
	uint8_t											  _activeObservers = 0; /// How many components are listening for notifications
	virtualRTC&										  _loggerRTC;
	std::array<char, 50>							  _timestamp{};		  /// Used to store the time and date of when the measurements were made
	TPluviometer									  _loggerPluviometer; ///
	TAnemometer										  _loggerAnemometer;
	uint16_t										  _rainInMm;
	uint16_t										  _windSpeedInMPS;

	/**
     * @brief iterates through array of listeners 
     * 
     * @param pStr 
     */
	void notify(const char* pStr, const char* _timestamp)
	{
		// no range loop because not all pointers in _listOfObservers are valid
		for (uint8_t i = 0; i < _activeObservers; i++)
		{
			_listOfObservers[i]->update(true, pStr, _timestamp);
		}
	}
};
