/**
 * @file gpio_module.c
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

#include "gpio_module.h"

////////////////////////////////////////////////////////////////////////
//							Functions declarations
////////////////////////////////////////////////////////////////////////

/**
 * @brief Initializes a GPIO pin as an output.
 *
 * @param pin The pin number to be initialized.
 * @param port The GPIO port to which the pin belongs.
 */
void initGPIOOutput(uint16_t pin, GPIO_TypeDef* port)
{
	// Assert that port is not NULL
	// assert(port != NULL);

	// Configure CS GPIO
	GPIO_InitTypeDef GPIO_InitStruct = {};

	if (port == GPIOF)
	{
		__HAL_RCC_GPIOF_CLK_ENABLE();
	}
	else if (port == GPIOE)
	{
		__HAL_RCC_GPIOE_CLK_ENABLE();
	}
	// Add other cases for different GPIO ports as needed

	else
	{
		// Error: Invalid GPIO port
		while (1);
		// assert(0);
	}

	GPIO_InitStruct.Pin	  = pin;
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(port, &GPIO_InitStruct);

	// Set the pin to a default state
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
}
