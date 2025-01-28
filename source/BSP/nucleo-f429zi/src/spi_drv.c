#include "spi_drv.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_spi.h"
#include <stm32f4xx_hal_gpio.h>

#define CS_PIN  GPIO_PIN_13
#define CS_PORT GPIOF

SPI_HandleTypeDef mySPIHandler;

void initCSPin(uint16_t pin, GPIO_TypeDef *port);

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

int8_t spi_init()
{
    initCSPin(CS_PIN, CS_PORT);

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
    
    if (HAL_OK != HAL_SPI_Init(&mySPIHandler))
    {
        while(1);
    }

    return 0;
}

uint16_t spi_transmit(uint8_t* pData, uint16_t size)
{
    if (HAL_OK != HAL_SPI_Transmit(&mySPIHandler, pData, size, HAL_MAX_DELAY))
    {
        return 0;
    }

    return size;
}

uint16_t spi_receive(uint8_t* pData, uint16_t size)
{
    if (HAL_OK != HAL_SPI_Receive(&mySPIHandler, pData, size, HAL_MAX_DELAY))
    {
        return 0;
    }

    return size;
}

uint16_t spi_transmitReceive(uint8_t* pTxData, uint8_t* pRxData, uint16_t size)
{
    if (HAL_OK != HAL_SPI_TransmitReceive(&mySPIHandler, pTxData, pRxData, size, HAL_MAX_DELAY))
    {
        return 0;
    }

    return size;
}

void csWrite(uint8_t val)
{
    if (1 == val)
    {
        HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);
    }
    else 
    {
        HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);
    }
}

/**
 * @brief Initializes a GPIO pin as an output.
 *
 * @param pin The pin number to be initialized.
 * @param port The GPIO port to which the pin belongs.
 */
void initCSPin(uint16_t pin, GPIO_TypeDef *port)
{
    // Assert that port is not NULL
    //assert(port != NULL);

    // Configure CS GPIO
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if (port == GPIOF)
    {
        __HAL_RCC_GPIOF_CLK_ENABLE();
    }
    else if (port == GPIOE)
    {
        __HAL_RCC_GPIOE_CLK_ENABLE();
    }
    // Add other cases for different GPIO ports as needed

    else
    {
        // Error: Invalid GPIO port
    	while(1);
        //assert(0);
    }

    GPIO_InitStruct.Pin = pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(port, &GPIO_InitStruct);

    // Set the pin to a default state
    HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
}
