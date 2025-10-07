#pragma once

#include <cstdint>

namespace ADC
{

template<typename T>
class IADC
{
  public:
	bool init()
	{
		return static_cast<T*>(this)->initImpl();
	}

	float readVoltage(uint8_t channel)
	{
		return static_cast<T*>(this)->readVoltageImpl(channel);
	}
};

}