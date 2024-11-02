#include "ads1115.hpp"
#include "init.h"

int main()
{
	// Init ADS1115
	ADS1115 myADS1115;

	// Init BSP
	stm32f429_init();

	for(;;)
	{
	}

	return 0;
}