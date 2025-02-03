/**
 * @file init.h
 * @author Renato Barresi (renatobarresi@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-02-03
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef INIT_H
#define INIT_H

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////
//							Function definition
////////////////////////////////////////////////////////////////////////

void stm32f429_init();

void SysTick_Handler(void);

#ifdef __cplusplus
}
#endif

#endif