#include "ethernet.h"
#include "stm32f4xx_hal.h"

ETH_TxPacketConfig TxConfig;
ETH_DMADescTypeDef DMARxDscrTab[ETH_RX_DESC_CNT]; /* Ethernet Rx DMA Descriptors */
ETH_DMADescTypeDef DMATxDscrTab[ETH_TX_DESC_CNT]; /* Ethernet Tx DMA Descriptors */

ETH_HandleTypeDef heth;

/**
  * @brief ETH Initialization Function
  * @param None
  * @retval None
  */
static void MX_ETH_Init(void)
{
	/* USER CODE BEGIN ETH_Init 0 */

	/* USER CODE END ETH_Init 0 */

	//static uint8_t MACAddr[6];

	/* USER CODE BEGIN ETH_Init 1 */

	/* USER CODE END ETH_Init 1 */
	heth.Instance = ETH;
	//  MACAddr[0] = 0x00;
	//  MACAddr[1] = 0x80;
	//  MACAddr[2] = 0xE1;
	//  MACAddr[3] = 0x00;
	//  MACAddr[4] = 0x00;
	//  MACAddr[5] = 0x00;
	//  heth.Init.MACAddr = &MACAddr[0];
	heth.Init.MediaInterface = HAL_ETH_RMII_MODE;
	//  heth.Init.TxDesc = DMATxDscrTab;
	//  heth.Init.RxDesc = DMARxDscrTab;
	//heth.Init.RxBuffLen = 1500;

	/* USER CODE BEGIN MACADDRESS */

	/* USER CODE END MACADDRESS */

	if (HAL_ETH_Init(&heth) != HAL_OK)
	{
		while (1);
	}

	//  memset(&TxConfig, 0 , sizeof(ETH_TxPacketConfig));
	//  TxConfig.Attributes = ETH_TX_PACKETS_FEATURES_CSUM | ETH_TX_PACKETS_FEATURES_CRCPAD;
	//  TxConfig.ChecksumCtrl = ETH_CHECKSUM_IPHDR_PAYLOAD_INSERT_PHDR_CALC;
	//  TxConfig.CRCPadCtrl = ETH_CRC_PAD_INSERT;
	/* USER CODE BEGIN ETH_Init 2 */

	/* USER CODE END ETH_Init 2 */
}

/**
* @brief ETH MSP Initialization
* This function configures the hardware resources used in this example
* @param heth: ETH handle pointer
* @retval None
*/
void HAL_ETH_MspInit(ETH_HandleTypeDef* hethMsp)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	if (hethMsp->Instance == ETH)
	{
		/* USER CODE BEGIN ETH_MspInit 0 */

		/* USER CODE END ETH_MspInit 0 */
		/* Peripheral clock enable */
		__HAL_RCC_ETH_CLK_ENABLE();

		__HAL_RCC_GPIOC_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();
		__HAL_RCC_GPIOG_CLK_ENABLE();
		/**ETH GPIO Configuration
    PC1     ------> ETH_MDC
    PA1     ------> ETH_REF_CLK
    PA2     ------> ETH_MDIO
    PA7     ------> ETH_CRS_DV
    PC4     ------> ETH_RXD0
    PC5     ------> ETH_RXD1
    PB13     ------> ETH_TXD1
    PG11     ------> ETH_TX_EN
    PG13     ------> ETH_TXD0
    */
		GPIO_InitStruct.Pin		  = GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5;
		GPIO_InitStruct.Mode	  = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull	  = GPIO_NOPULL;
		GPIO_InitStruct.Speed	  = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

		GPIO_InitStruct.Pin		  = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_7;
		GPIO_InitStruct.Mode	  = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull	  = GPIO_NOPULL;
		GPIO_InitStruct.Speed	  = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin		  = GPIO_PIN_13;
		GPIO_InitStruct.Mode	  = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull	  = GPIO_NOPULL;
		GPIO_InitStruct.Speed	  = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		GPIO_InitStruct.Pin		  = GPIO_PIN_11 | GPIO_PIN_13;
		GPIO_InitStruct.Mode	  = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull	  = GPIO_NOPULL;
		GPIO_InitStruct.Speed	  = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
		HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

		/* ETH interrupt Init */
		HAL_NVIC_SetPriority(ETH_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(ETH_IRQn);
		/* USER CODE BEGIN ETH_MspInit 1 */

		/* USER CODE END ETH_MspInit 1 */
	}
}

int8_t eth_init()
{
	MX_ETH_Init();
	return 1;
}