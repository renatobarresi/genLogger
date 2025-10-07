#include "aht21_wrapper.hpp"

namespace sensor::termometer
{

bool AHT21::initImpl()
{
	return true;
}

std::optional<float> AHT21::readTemperatureImpl()
{
	return 25.5;
}

} // namespace sensor::termometer