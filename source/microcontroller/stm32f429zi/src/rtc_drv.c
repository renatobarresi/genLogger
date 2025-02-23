#include "rtc_drv.h"

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_rtc.h"

RTC_HandleTypeDef hrtc;

/**
* @brief RTC MSP Initialization
* This function configures the hardware resources used in this example
* @param hrtc: RTC handle pointer
* @retval None
*/
void HAL_RTC_MspInit(RTC_HandleTypeDef* rtc)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  if(rtc->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspInit 0 */

  /* USER CODE END RTC_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      while(1);
    }

    /* Peripheral clock enable */
    __HAL_RCC_RTC_ENABLE();
  /* USER CODE BEGIN RTC_MspInit 1 */

  /* USER CODE END RTC_MspInit 1 */

  }

}

int8_t rct_init(void)
{
    hrtc.Instance = RTC;
    hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
    hrtc.Init.AsynchPrediv = 127;
    hrtc.Init.SynchPrediv = 255;
    hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
    hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
    
    if (HAL_RTC_Init(&hrtc) != HAL_OK)
    {
        return -1;
    }

    return 1;
}

int8_t rtc_set_time(uint8_t hour, uint8_t minute, uint8_t second)
{
  RTC_TimeTypeDef sTime = {0};

  sTime.Hours = hour;
  sTime.Minutes = minute;
  sTime.Seconds = second;

  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    return -1;
  }

  return 1;
}

void rtc_get_time(uint8_t *hour, uint8_t *minute, uint8_t *second)
{
  RTC_TimeTypeDef sTime = {0};

  HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);

  *hour = sTime.Hours;
  *minute = sTime.Minutes;
  *second = sTime.Seconds;
}

int8_t rtc_set_date(uint8_t year, uint8_t month, uint8_t day)
{
  RTC_DateTypeDef sDate = {0};

  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = month;
  sDate.Date = day;
  sDate.Year = year;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
  {
    return -1;
  }

  return 1;
}

void rtc_get_date(uint8_t *year, uint8_t *month, uint8_t *day)
{
  RTC_DateTypeDef sDate = {0};

  HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
  
  *year = sDate.Year;
  *month = sDate.Month;
  *day = sDate.Date; 
}
