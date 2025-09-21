#pragma once
#include "virtualADC.hpp"

namespace ADC
{
class ADS1115 : public ADC::IADC<ADS1115>
{
  public:
	bool  initImpl();
	float readVoltageImpl(uint8_t channel);
};
}