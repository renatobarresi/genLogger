#include "i2c_drv.h"
#include "stm32f4xx_hal.h"
#include <stm32f4xx_hal_def.h>
#include <stm32f4xx_hal_i2c.h>


int8_t  stm32f429_i2cInit(i2c_dev_t* i2cDev)
{
	i2cDev->i2cHandle.Instance = i2cDev->i2cInstance;
	i2cDev->i2cHandle.Init.ClockSpeed = 100000;
	i2cDev->i2cHandle.Init.DutyCycle = I2C_DUTYCYCLE_2;
	i2cDev->i2cHandle.Init.OwnAddress1 = 0;
	i2cDev->i2cHandle.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	i2cDev->i2cHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	i2cDev->i2cHandle.Init.OwnAddress2 = 0;
	i2cDev->i2cHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	i2cDev->i2cHandle.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

	if (HAL_I2C_Init(&i2cDev->i2cHandle) != HAL_OK)
	{
		return -1;
	}
	/** Configure Analogue filter
	 */
	if (HAL_I2CEx_ConfigAnalogFilter(&i2cDev->i2cHandle, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
	{
		return -1;
	}
	/** Configure Digital filter
	 */
	if (HAL_I2CEx_ConfigDigitalFilter(&i2cDev->i2cHandle, 0) != HAL_OK)
	{
		return -1;
	}

	return 1;
}

int8_t stm32f429_mem_i2cTx(i2c_dev_t* i2cDev, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    if (HAL_OK != HAL_I2C_Mem_Write(&i2cDev->i2cHandle, DevAddress, MemAddress, MemAddSize, pData, Size, Timeout))
    {
        return -1;
    }

    return 1;
}

int8_t stm32f429_mem_i2cRx(i2c_dev_t* i2cDev, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    if (HAL_OK != HAL_I2C_Mem_Read(&i2cDev->i2cHandle, DevAddress, MemAddress, MemAddSize, pData, Size, Timeout))
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

  if(hi2c->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspInit 0 */

  /* USER CODE END I2C1_MspInit 0 */

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C1 GPIO Configuration
    PB8     ------> I2C1_SCL
    PB9     ------> I2C1_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Peripheral clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();

  /* USER CODE BEGIN I2C1_MspInit 1 */

  /* USER CODE END I2C1_MspInit 1 */
  }

}
