#pragma once

#include "driver_aht21.h"
#include "interfaceTermometer.hpp"

namespace sensor::termometer
{

class AHT21 : public Itermometer<AHT21>
{
  public:
	bool				 initImpl();
	std::optional<float> readTemperatureImpl();

  private:
	aht21_handle_t _gs_handle;
};

} // namespace sensor::termometer