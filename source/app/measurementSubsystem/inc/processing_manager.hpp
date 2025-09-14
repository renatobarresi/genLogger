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

constexpr uint8_t  MAX_NUM_OBSERVERS   = 10;
constexpr uint16_t MSRD_DATA_BUFF_SIZE = 1024;

class observerInterface // change class name, maybe processingMngObserver or IProcessing
{
  public:
	virtual void update(bool infoFlag) = 0;
	virtual ~observerInterface()	   = default;
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
		sprintf(_sensorInfoBuff.data(), "%d;%d;%s\n", _rainInMm, _windSpeedInMPS, _timestamp.data());
	}

	void notifyObservers()
	{
		notify(_sensorInfoBuff.data());
	}

	const char* getSensorInfoBuff()
	{
		return _sensorInfoBuff.data();
	}

  private:
	std::array<char, MSRD_DATA_BUFF_SIZE>			  _sensorInfoBuff;
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
	void notify(const char* pBuff)
	{
		// no range loop because not all pointers in _listOfObservers are valid
		for (uint8_t i = 0; i < _activeObservers; i++)
		{
			_listOfObservers[i]->update(true);
		}
	}
};
