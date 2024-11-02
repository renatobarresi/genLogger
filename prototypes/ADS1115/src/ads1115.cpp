/**
 * @file ads1115.cpp
 * @author Renato Barresi (renatobarresi@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-10-30
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "ads1115.hpp"
#include "i2cInterface.h"

ADS1115::ADS1115() {}

ADS1115::error_t ADS1115::init()
{
	error_t retVal = ADS1115_noError;

	return retVal;
}

ADS1115::error_t ADS1115::setMode(error_t mode)
{
	error_t retVal = ADS1115_noError;

	return retVal;
}

ADS1115::error_t ADS1115::setVoltRange()
{
	error_t retVal = ADS1115_noError;

	return retVal;
}
ADS1115::error_t ADS1115::setCompChannel(compareChannel_t compChannel)
{
	error_t retVal = ADS1115_noError;

	return retVal;
}

void ADS1115::startSingleMeasurement() {}

float ADS1115::getVoltage()
{
	float voltage = 0.0;

	return voltage;
}