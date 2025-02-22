#include "stm32F429_RTC.hpp"

bool stm429RTC::init()
{
	return true;
}
bool stm429RTC::setTime(uint8_t hour, uint8_t minute, uint8_t seconds)
{
	return true;
}
void stm429RTC::getTime(char* buffer, size_t bufferSize) const
{
	//
}
bool stm429RTC::setDate(uint8_t day, uint8_t month, uint16_t year)
{
	return true;
}
void stm429RTC::getDate(char* buffer, size_t bufferSize) const
{
	//
}