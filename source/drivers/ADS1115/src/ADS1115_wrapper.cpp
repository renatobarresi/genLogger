#include "ADS1115_wrapper.hpp"
#include "ADS1115.h"

namespace ADC
{

ADS1115_handler ADS1115_1;

bool ADS1115::initImpl()
{
	if (0 == ADS1115_init(&ADS1115_1, ADS1115_I2C_ADDR_GND))
	{
		return false;
	}

	if (0 == ADS1115_Set_Mode(&ADS1115_1, ADS1115_SINGLE_SHOT))
	{
		return false;
	}

	if (0 == ADS1115_Set_Volt_Range(&ADS1115_1, ADS1115_RANGE_4096))
	{
		return false;
	}

	if (0 == ADS1115_Set_Compare_Channel(&ADS1115_1, ADS1115_COMP_0_GND))
	{
		return false;
	}

	return true;
}

float ADS1115::readVoltageImpl()
{
	float retVal = 0;

	if (0 == ADS1115_Start_Single_Measurement(&ADS1115_1))
	{
		return retVal;
	}

	retVal = ADS1115_Get_Volt(&ADS1115_1);

	return retVal;
}
} // namespace ADC