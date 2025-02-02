#include "leds.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"


#define RED_LED_PORT GPIOB
#define RED_LED_PIN GPIO_PIN_14

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