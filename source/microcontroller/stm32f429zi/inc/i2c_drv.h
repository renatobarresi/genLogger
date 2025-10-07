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

////////////////////////////////////////////////////////////////////////
//							    Includes
////////////////////////////////////////////////////////////////////////

#include <stdint.h>

////////////////////////////////////////////////////////////////////////
//							    Types
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
//							Function definition
////////////////////////////////////////////////////////////////////////

int8_t i2c_init();

int8_t i2c_deinit();

int8_t i2c_write(uint16_t devAddr, uint16_t memAddr, uint8_t* pData, uint16_t size);

int8_t i2c_read(uint16_t devAddr, uint16_t memAddr, uint8_t* pData, uint16_t size);

#ifdef __cplusplus
}
#endif

#endif
