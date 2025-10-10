#pragma once
#include <optional>

namespace sensor::thermometer
{

template<typename T>
class IThermometer
{
  public:
	bool init()
	{
		return static_cast<T*>(this)->initImpl();
	}

	std::optional<float> readTemperature()
	{
		return static_cast<T*>(this)->readTemperatureImpl();
	}
};

} // namespace sensor::thermometer