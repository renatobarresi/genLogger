#include "timer.h"
#include "stdint.h"
#include "stm32f4xx_hal.h"

static volatile uint64_t ticksSinceStart = 0;

void HAL_IncTick(void)
{
	ticksSinceStart++;
}

// TODO, remove casting
uint32_t HAL_GetTick(void)
{
	return (uint32_t)ticksSinceStart;
}

uint64_t timer_getTick()
{
	return ticksSinceStart;
}