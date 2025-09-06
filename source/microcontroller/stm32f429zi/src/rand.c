#include "rand.h"

#include "stm32f4xx_hal.h"

static RNG_HandleTypeDef hrng;

int8_t rand_init()
{
	hrng.Instance = RNG;
	if (HAL_RNG_Init(&hrng) != HAL_OK)
	{
		return -1;
	}

	return 1;
}

void rand_generate_random_number(uint32_t* num)
{
	HAL_RNG_GenerateRandomNumber(&hrng, num);
}
