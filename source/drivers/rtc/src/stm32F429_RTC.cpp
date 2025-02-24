#include "stm32F429_RTC.hpp"
#include "rtc_drv.h"
#include <cstdio>

bool stm429RTC::init()
{
	return true;
}
bool stm429RTC::setTime(uint8_t hour, uint8_t minute, uint8_t seconds)
{
	if (-1 == rtc_set_time(hour, minute, seconds))
	{
		return false;
	}

	return true;
}
void stm429RTC::getTime(char* buffer, size_t bufferSize) const
{
	uint8_t minutes = 0;
	uint8_t hours	= 0;
	uint8_t seconds = 0;

	rtc_get_date(&hours, &minutes, &seconds);

	snprintf(buffer, bufferSize, "%02d:%02d:%02d", hours, minutes, seconds);
}

bool stm429RTC::setDate(uint8_t day, uint8_t month, uint16_t year)
{
	return true;
}

void stm429RTC::getDate(char* buffer, size_t bufferSize) const
{
	//
}