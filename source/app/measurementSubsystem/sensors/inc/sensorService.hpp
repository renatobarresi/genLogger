#pragma once

#include <cstdint>

class sensorServiceInterface
{
  public:
	virtual float requestADCVoltage() = 0;
	virtual ~sensorServiceInterface() = default;
};

template<typename ADC, typename TThermometer, typename THygrometer>
class sensorService : public sensorServiceInterface
{
  public:
	// clang-format off
	sensorService(ADC& adc,
				  TThermometer& thermometer,
		  		  THygrometer& hygrometer) :
				  _adc(adc),
				  _thermometer(thermometer),
				  _hygrometer(hygrometer) {}
	// clang-format on

	float requestADCVoltage()
	{
		return _adc.readVoltage(0);
	}

	float requestTemperature()
	{
		return _thermometer.readTemperature();
	}

	uint8_t requestHumidity()
	{
		return _hygrometer.readHumidity();
	}

  private:
	ADC&		  _adc;
	TThermometer& _thermometer;
	THygrometer&  _hygrometer;
};