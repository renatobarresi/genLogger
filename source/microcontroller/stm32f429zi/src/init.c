/**
 * @file init.c
 * @author Renato Barresi (renatobarresi@gmail.com)
 * @brief This is the source file for all microncontroller/system
 * initialization functions, all init work is done here.
 *
 * It is the responsability of this subsystem to initialize all
 * and peripherals, HAL, system clock, etc. All layers depend on the
 * correct initialization of this subsystem.
 *
 * @version 0.1
 * @date 2025-02-03
 * 
 * @copyright Copyright (c) 2025
 * 
 */

////////////////////////////////////////////////////////////////////////
//							    Includes
////////////////////////////////////////////////////////////////////////

#include "init.h"
#include "stm32f4xx_hal.h"
//#include "i2c_drv.h"
#include "uart.h"
#include "spi_drv.h"
#include "rtc_drv.h"

////////////////////////////////////////////////////////////////////////
//							Functions definitions
////////////////////////////////////////////////////////////////////////

void SystemClock_Config(void);
void MX_GPIO_Init(void);
void HAL_MspInit(void);

////////////////////////////////////////////////////////////////////////
//							Private function definitions
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
//							Functions declarations
////////////////////////////////////////////////////////////////////////

void stm32f429_init()
{
  // Init ST Hal if used
  if (HAL_OK != HAL_Init())
  {
    while(1);
  }

  // Init clock
  SystemClock_Config();

  // Init peripherals
  MX_GPIO_Init();

  // Init DMA 
  MX_DMA_Init();

  if ( -1 == uart_init())
  {
    while(1);
  }

  if (-1 == spi_init())
  {
    while(1);
  }

  if (-1 == rtc_init())
  {
    while(1);
  }

  //stm32f429_i2cInit(&i2c1Device);
}

void HAL_MspInit(void)
{
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    for(;;);
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    for(;;);
  }
}

void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOB_CLK_ENABLE();

    /*Configure GPIO pins : LD1_Pin LD3_Pin LD2_Pin */
    
    GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_0|GPIO_PIN_14;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

