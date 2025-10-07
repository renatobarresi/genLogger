#pragma once

#include <cstdint>
#include <virtualADC.hpp>

namespace sensor
{

template<typename A>
class IAnemometer
{
  public:
	uint16_t getWindSpeed()
	{
		return static_cast<A*>(this)->getWindSpeedImpl();
	}
};

class anemometerDavis : public IAnemometer<anemometerDavis>
{
  public:
	uint16_t getWindSpeedImpl()
	{
		return 0; // todo
	}
};

template<typename V>
class IWindVane
{
  public:
	uint16_t getWindDir()
	{
		return static_cast<V*>(this)->getWindDirImpl();
	}
};

template<typename ADC>
class windVaneDavis : public IWindVane<windVaneDavis<ADC>>
{
  public:
	windVaneDavis(ADC& adc) : _adc(adc) {}
	uint16_t getWindDirImpl()
	{
		return 0; // todo
	}

  private:
	ADC& _adc;
};

}; // namespace sensor
