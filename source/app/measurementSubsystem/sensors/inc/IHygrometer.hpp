#pragma once

#include <cstdint>
#include <optional>

namespace sensor::hygrometer
{

template<typename H>
class IHygrometer
{
  public:
	bool initHygrometer()
	{
		return static_cast<H*>(this)->initImpl();
	}

	std::optional<uint8_t> readHumidity()
	{
		return static_cast<H*>(this)->readHumidityImpl();
	}
};

} // namespace sensor::hygrometer