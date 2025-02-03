/**
 * @file spi_drv.h
 * @author Renato Barresi (renatobarresi@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-02-03
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef SPI_DRV_H
#define SPI_DRV_H

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////
//							    Includes
////////////////////////////////////////////////////////////////////////

#include "stdint.h"

////////////////////////////////////////////////////////////////////////
//							Function definition
////////////////////////////////////////////////////////////////////////

/**
 * @brief Initialize the SPI module.
 *
 * @return The initialization status.
 */
int8_t spi_init();

/**
 * @brief Transmit data over SPI.
 *
 * @param pData Pointer to the data buffer to transmit.
 * @param size The size of the data buffer.
 * @return The transmission status.
 */
uint16_t spi_transmit(uint8_t* pData, uint16_t size);

/**
 * @brief Receive data over SPI.
 *
 * @param pData Pointer to the data buffer to receive.
 * @param size The size of the data buffer.
 * @return The reception status.
 */
uint16_t spi_receive(uint8_t* pData, uint16_t size);

/**
 * @brief Transmit and receive data over SPI simultaneously.
 *
 * @param pTxData Pointer to the transmit data buffer.
 * @param pRxData Pointer to the receive data buffer.
 * @param size The size of the data buffers.
 * @return The transmission/reception status.
 */
uint16_t spi_transmitReceive(uint8_t* pTxData, uint8_t* pRxData, uint16_t size);

/**
 * @brief Set the chip select pin to high.
 *
 * @param pin The pin number of the chip select.
 * @param port The GPIO port of the chip select.
 */
void csWrite(uint8_t val);

#ifdef __cplusplus
}
#endif
#endif
