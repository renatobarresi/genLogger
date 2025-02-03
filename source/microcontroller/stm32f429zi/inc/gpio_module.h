/**
 * @file gpio_module.h
 * @author Renato Barresi (renatobarresi@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-02-03
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef GPIO_MODULE_H
#define GPIO_MODULE_H

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////
//							    Includes
////////////////////////////////////////////////////////////////////////

#include "stm32f4xx_hal.h"

////////////////////////////////////////////////////////////////////////
//							Function definition
////////////////////////////////////////////////////////////////////////

void initGPIOOutput(uint16_t pin, GPIO_TypeDef* port);

#ifdef __cplusplus
}
#endif

#endif /* INC_GPIO_MODULE_H_ */
