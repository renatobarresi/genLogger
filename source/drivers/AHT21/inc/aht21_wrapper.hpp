#pragma once

#include "IHygrometer.hpp"
#include "IThermometer.hpp"
#include "driver_aht21.h"

namespace sensor::thermometer
{

class AHT21 : public IThermometer<AHT21>, public hygrometer::IHygrometer<AHT21>
{
  public:
	bool				   initImpl();
	std::optional<float>   readTemperatureImpl();
	std::optional<uint8_t> readHumidityImpl();

  private:
	aht21_handle_t _gs_handle;
};

} // namespace sensor::thermometer