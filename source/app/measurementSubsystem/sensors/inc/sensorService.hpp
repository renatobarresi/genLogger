#pragma once

class sensorServiceInterface
{
  public:
	virtual float requestADCVoltage() = 0;
	virtual ~sensorServiceInterface() = default;
};

template<typename ADC>
class sensorService : public sensorServiceInterface
{
  public:
	sensorService(ADC& adc) : _adc(adc) {}
	float requestADCVoltage()
	{
		return _adc.readVoltage(0);
	}

  private:
	ADC& _adc;
};