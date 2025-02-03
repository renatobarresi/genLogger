*/**
 * @file leds.c
 * @author Renato Barresi (renatobarresi@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-02-03
 * 
 * @copyright Copyright (c) 2025
 * 
 */

////////////////////////////////////////////////////////////////////////
//							    Includes
////////////////////////////////////////////////////////////////////////

#include "leds.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"

////////////////////////////////////////////////////////////////////////
//							    Defines
////////////////////////////////////////////////////////////////////////

#define RED_LED_PORT GPIOB
#define RED_LED_PIN GPIO_PIN_14

////////////////////////////////////////////////////////////////////////
//							    Functions
////////////////////////////////////////////////////////////////////////

void turnOffRedLed()
{
    HAL_GPIO_WritePin(RED_LED_PORT, RED_LED_PIN, GPIO_PIN_RESET);
}

void turnOnRedLed()
{
    HAL_GPIO_WritePin(RED_LED_PORT, RED_LED_PIN, GPIO_PIN_SET);
}

void ledDelay(int delay)
{
    HAL_Delay((uint32_t) delay);
}