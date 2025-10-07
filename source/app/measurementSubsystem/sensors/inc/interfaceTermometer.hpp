#pragma once
#include <optional>

namespace sensor::termometer
{

template<typename T>
class Itermometer
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

} // namespace sensor::termometer