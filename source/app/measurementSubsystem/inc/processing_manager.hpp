#pragma once

// #include "anemometer.hpp"
// #include "pluviometer.hpp"
#include "IHygrometer.hpp"
#include "IThermometer.hpp"
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
	virtual void update()		 = 0;
	virtual ~observerInterface() = default;
};

/**
 * @brief 
 * 
 * @tparam TPluviometer 
 * @tparam TAnemometer 
 */
//template<typename TPluviometer, typename TAnemometer, typename TWindVane>
template<typename TThermometer, typename THygrometer>
class processingManager
{
  public:
	/**
	 * @brief constructor
	 */
	// clang-format off
	processingManager(virtualRTC& rtc,
					  TThermometer& thermometer,
					  THygrometer& hygrometer)
					  /*TPluviometer& pluviometer,
					  TAnemometer& anemometer,
					  TWindVane& windVane*/ :
					  _loggerRTC(rtc),
					  _thermometer(thermometer),
					  _hygrometer(hygrometer)
					  /*_loggerPluviometer(pluviometer), 
					  _loggerAnemometer(anemometer), 
					  _loggerWindVane(windVane)*/ {}
	// clang-format on
	void init(void)
	{
#ifndef TARGET_MICRO
		//sensorSimulator::init();
#endif
		/* Init sensors */
		_thermometer.init();
		_hygrometer.initHygrometer();
	}

	void setObserver(observerInterface* ref)
	{
		_listOfObservers[_activeObservers++] = ref;
	}

	void takeMeasurements()
	{
		_loggerRTC.getTimestamp(_timestamp.data());

		// TODO all sensors
		_temperature = _thermometer.readTemperature().value_or(255);
		_humidity	 = _hygrometer.readHumidity().value_or(255);
		// _rainInMm		= _loggerPluviometer.getRain();
		// _windSpeedInMPS = _loggerAnemometer.getWindSpeed();
		// _windDir		= _loggerWindVane.getWindDir();
	}

	void formatData()
	{
		// Append measurements with time they were taken
		sprintf(_sensorInfoBuff.data(), "%s;%d;%d\n", _timestamp.data(), _temperature, _humidity);
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
	std::array<char, 50>							  _timestamp{}; /// Used to store the time and date of when the measurements were made
	TThermometer&									  _thermometer;
	THygrometer&									  _hygrometer;
	// TPluviometer&								  _loggerPluviometer;
	// TAnemometer&									  _loggerAnemometer;
	// TWindVane&									  _loggerWindVane;

	std::optional<float>   _temperature;
	std::optional<uint8_t> _humidity;
	// uint16_t _rainInMm;
	// uint16_t _windSpeedInMPS;
	// uint16_t _windDir;

	/**
     * @brief iterates through array of listeners 
     * 
     * @param pStr 
     */
	void notify(const char* pBuff)
	{
		(void)pBuff;
		// no range loop because not all pointers in _listOfObservers are valid
		for (uint8_t i = 0; i < _activeObservers; i++)
		{
			_listOfObservers[i]->update();
		}
	}
};
