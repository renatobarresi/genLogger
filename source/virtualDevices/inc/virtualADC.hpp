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

	float readVoltage()
	{
		return static_cast<T*>(this)->readVoltageImpl();
	}
};

}