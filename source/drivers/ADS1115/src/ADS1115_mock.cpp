#include "ADS1115_wrapper.hpp"

namespace ADC
{
bool ADS1115::initImpl()
{
	return true;
}

float ADS1115::readVoltageImpl(uint8_t channel)
{
	return 3.3f;
}

} // namespace ADC