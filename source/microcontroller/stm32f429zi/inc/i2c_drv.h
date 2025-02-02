/**
 * @file i2c_drv.h
 * @author Renato Barresi (renatobarresi@gmail.com)
 * @brief Heaeder file with functions to control an STM32F429zi I2C peripheral
		  this module depends on ST's HAL.
 * @version 0.1
 * @date 2024-11-01
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef I2C_HEADER_H
#define I2C_HEADER_H

#ifdef __cplusplus
extern "C" {
#endif
#include "stdint.h"
#include "stm32f4xx_hal_dma.h"
#include <stm32f4xx_hal_i2c.h>

typedef struct stm32f429_i2c_dev_t
{
	I2C_HandleTypeDef i2cHandle;
	I2C_TypeDef* i2cInstance;
} i2c_dev_t;

void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c);

int8_t stm32f429_i2cInit(i2c_dev_t* i2cDev);

int8_t stm32f429_mem_i2cTx(i2c_dev_t* i2cDev, uint16_t DevAddress, uint16_t MemAddress,
						   uint16_t MemAddSize, uint8_t* pData, uint16_t Size, uint32_t Timeout);

int8_t stm32f429_mem_i2cRx(i2c_dev_t* i2cDev, uint16_t DevAddress, uint16_t MemAddress,
						   uint16_t MemAddSize, uint8_t* pData, uint16_t Size, uint32_t Timeout);

#ifdef __cplusplus
}
#endif

#endif
