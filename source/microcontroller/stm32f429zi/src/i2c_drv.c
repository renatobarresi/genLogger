/**
 * @file i2c_drv.c
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

#include "i2c_drv.h"
#include "stm32f4xx_hal.h"
#include <stm32f4xx_hal_i2c.h>

////////////////////////////////////////////////////////////////////////
//							Private function definitions
////////////////////////////////////////////////////////////////////////

static I2C_HandleTypeDef hi2c1;

////////////////////////////////////////////////////////////////////////
//							Functions declarations
////////////////////////////////////////////////////////////////////////

int8_t i2c_init(void)
{
	hi2c1.Instance			   = I2C1;
	hi2c1.Init.ClockSpeed	   = 100000;
	hi2c1.Init.DutyCycle	   = I2C_DUTYCYCLE_2;
	hi2c1.Init.OwnAddress1	   = 0;
	hi2c1.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2	   = 0;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;

	if (HAL_I2C_Init(&hi2c1) != HAL_OK)
	{
		return -1;
	}
	/** Configure Analogue filter
	 */
	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
	{
		return -1;
	}
	/** Configure Digital filter
	 */
	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
	{
		return -1;
	}

	return 1;
}

/**
* @brief I2C MSP Initialization
* This function configures the hardware resources used in this example
* @param hi2c: I2C handle pointer
* @retval None
*/
void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	if (hi2c->Instance == I2C1)
	{
		/* USER CODE BEGIN I2C1_MspInit 0 */

		/* USER CODE END I2C1_MspInit 0 */

		__HAL_RCC_GPIOB_CLK_ENABLE();
		/**I2C1 GPIO Configuration
    PB8     ------> I2C1_SCL
    PB9     ------> I2C1_SDA
    */
		GPIO_InitStruct.Pin		  = GPIO_PIN_8 | GPIO_PIN_9;
		GPIO_InitStruct.Mode	  = GPIO_MODE_AF_OD;
		GPIO_InitStruct.Pull	  = GPIO_NOPULL;
		GPIO_InitStruct.Speed	  = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		/* Peripheral clock enable */
		__HAL_RCC_I2C1_CLK_ENABLE();

		/* USER CODE BEGIN I2C1_MspInit 1 */

		/* USER CODE END I2C1_MspInit 1 */
	}
}

int8_t i2c_write(uint16_t devAddr, uint16_t memAddr, uint8_t* pData, uint16_t size)
{
	HAL_StatusTypeDef halRetVal;
	int8_t			  retVal;

	halRetVal = HAL_I2C_Mem_Write(&hi2c1, devAddr, memAddr, sizeof(uint8_t), pData, 2, HAL_MAX_DELAY);

	if (halRetVal == HAL_ERROR)
	{
		retVal = -1;
	}
	else
	{
		retVal = 1;
	}

	return retVal;
}

int8_t i2c_read(uint16_t devAddr, uint16_t memAddr, uint8_t* pData, uint16_t size)
{
	HAL_StatusTypeDef halRetVal;
	int8_t			  retVal;

	halRetVal = HAL_I2C_Mem_Read(&hi2c1, devAddr, memAddr, sizeof(uint8_t), pData, 2, HAL_MAX_DELAY);

	if (halRetVal == HAL_ERROR)
	{
		retVal = -1;
	}
	else
	{
		retVal = 1;
	}

	return retVal;
}
