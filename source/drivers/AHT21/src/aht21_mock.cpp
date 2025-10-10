#include "aht21_wrapper.hpp"

namespace sensor::thermometer
{

bool AHT21::initImpl()
{
	return true;
}

std::optional<float> AHT21::readTemperatureImpl()
{
	return 25.5;
}

std::optional<uint8_t> AHT21::readHumidityImpl()
{
	return 70;
}

} // namespace sensor::thermometer