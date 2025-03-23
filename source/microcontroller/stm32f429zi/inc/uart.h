/**
 * @file uart.h
 * @author Renato Barresi (renatobarresi@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-02-03
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef UART_H
#define UART_H

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////
//							    Includes
////////////////////////////////////////////////////////////////////////

#include <stdint.h>

////////////////////////////////////////////////////////////////////////
//							Function definition
////////////////////////////////////////////////////////////////////////

int8_t uart_init();

void MX_DMA_Init(void);

void uart_tx(char pBuff, int buffSize);

void uart_start_rx_dma();

uint8_t uart_isDataAvailable();

void uart_getAvailableData(uint8_t* pBuff, uint16_t buffSize);

#ifdef __cplusplus
}
#endif

#endif