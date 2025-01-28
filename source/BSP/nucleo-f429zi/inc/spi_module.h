/*
 * spi_module.h
 *
 *  Created on: Apr 5, 2023
 *      Author: renatobarresi
 */

#ifndef UART_H
#define UART_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"

/* Global variables */

/* Types */

/**
 * @brief Initialize the SPI module.
 *
 * @return The initialization status.
 */
HAL_StatusTypeDef init();

/**
 * @brief Transmit data over SPI.
 *
 * @param pData Pointer to the data buffer to transmit.
 * @param size The size of the data buffer.
 * @return The transmission status.
 */
HAL_StatusTypeDef transmit(uint8_t* pData, uint16_t size);

/**
 * @brief Receive data over SPI.
 *
 * @param pData Pointer to the data buffer to receive.
 * @param size The size of the data buffer.
 * @return The reception status.
 */
HAL_StatusTypeDef receive(uint8_t* pData, uint16_t size);

/**
 * @brief Transmit and receive data over SPI simultaneously.
 *
 * @param pTxData Pointer to the transmit data buffer.
 * @param pRxData Pointer to the receive data buffer.
 * @param size The size of the data buffers.
 * @return The transmission/reception status.
 */
HAL_StatusTypeDef transmitReceive(uint8_t* pTxData, uint8_t* pRxData, uint16_t size);

/**
 * @brief Set the chip select pin to high.
 *
 * @param pin The pin number of the chip select.
 * @param port The GPIO port of the chip select.
 */
void csHigh(uint16_t pin, GPIO_TypeDef* port);

/**
 * @brief Set the chip select pin to low.
 *
 * @param pin The pin number of the chip select.
 * @param port The GPIO port of the chip select.
 */
void csLow(uint16_t pin, GPIO_TypeDef* port);

#ifdef __cplusplus
}
#endif
#endif
