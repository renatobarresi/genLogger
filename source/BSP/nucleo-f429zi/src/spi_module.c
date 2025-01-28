/*
 * spi_module.cpp
 *
 *  Created on: Apr 5, 2023
 *      Author: renatobarresi
 *
 *  This file contains the implementation of the SPI module class for STM32F4 microcontrollers.
 */

#include "spi_module.h"

/* Global variables */

SPI_HandleTypeDef mySPIHandler;

/* Peripheral initialization */
/**
 * @brief SPI MSP Initialization
 * This function configures the hardware resources used in this example.
 *
 * @param hspi: SPI handle pointer.
 * @retval None.
 */
void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	if(hspi->Instance == SPI1)
	{
		/* USER CODE BEGIN SPI1_MspInit 0 */

		/* USER CODE END SPI1_MspInit 0 */
		/* Peripheral clock enable */
		__HAL_RCC_SPI1_CLK_ENABLE();

		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();
		/**SPI1 GPIO Configuration
		PA5     ------> SPI1_SCK
		PA6     ------> SPI1_MISO
		PB5     ------> SPI1_MOSI
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_5;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	}
}

/**
 * @brief Initialize the SPI module.
 *
 * @return The initialization status.
 */
HAL_StatusTypeDef init()
{
	HAL_StatusTypeDef status;

	mySPIHandler.Instance = SPI1; // TODO add modularity
	mySPIHandler.Init.Mode = SPI_MODE_MASTER;
	mySPIHandler.Init.Direction = SPI_DIRECTION_2LINES;
	mySPIHandler.Init.DataSize = SPI_DATASIZE_8BIT;
	mySPIHandler.Init.CLKPolarity = SPI_POLARITY_LOW;
	mySPIHandler.Init.CLKPhase = SPI_PHASE_1EDGE;
	mySPIHandler.Init.NSS = SPI_NSS_SOFT;
	mySPIHandler.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
	mySPIHandler.Init.FirstBit = SPI_FIRSTBIT_MSB;
	mySPIHandler.Init.TIMode = SPI_TIMODE_DISABLE;
	mySPIHandler.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	mySPIHandler.Init.CRCPolynomial = 10;

	status = HAL_SPI_Init(&mySPIHandler);

	if (status != HAL_OK)
	{
		while(1);
	}

	return status;
}

/**
 * @brief Transmit data over SPI.
 *
 * @param pData Pointer to the data buffer to transmit.
 * @param size The size of the data buffer.
 * @return The transmission status.
 */
HAL_StatusTypeDef transmit(uint8_t* pData, uint16_t size)
{
	return HAL_SPI_Transmit(&mySPIHandler, pData, size, HAL_MAX_DELAY);
}

/**
 * @brief Receive data over SPI.
 *
 * @param pData Pointer to the data buffer to receive.
 * @param size The size of the data buffer.
 * @return The reception status.
 */
HAL_StatusTypeDef receive(uint8_t* pData, uint16_t size)
{
	return HAL_SPI_Receive(&mySPIHandler, pData, size, HAL_MAX_DELAY);
}

/**
 * @brief Transmit and receive data over SPI simultaneously.
 *
 * @param pTxData Pointer to the transmit data buffer.
 * @param pRxData Pointer to the receive data buffer.
 * @param size The size of the data buffers.
 * @return The transmission/reception status.
 */
HAL_StatusTypeDef transmitReceive(uint8_t* pTxData, uint8_t* pRxData, uint16_t size)
{
	return HAL_SPI_TransmitReceive(&mySPIHandler, pTxData, pRxData, size, HAL_MAX_DELAY);
}

/**
 * @brief Set the chip select pin to high.
 *
 * @param pin The pin number of the chip select.
 * @param port The GPIO port of the chip select.
 */
void csHigh(uint16_t pin, GPIO_TypeDef* port)
{
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
}

/**
 * @brief Set the chip select pin to low.
 *
 * @param pin The pin number of the chip select.
 * @param port The GPIO port of the chip select.
 */
void csLow(uint16_t pin, GPIO_TypeDef* port)
{
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
}
