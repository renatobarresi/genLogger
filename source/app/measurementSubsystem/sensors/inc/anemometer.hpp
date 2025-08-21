#pragma once

#include <cstdint>

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

}; // namespace sensor
