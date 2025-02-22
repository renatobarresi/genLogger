#pragma once

#ifdef TARGET_MICRO
#include "stm32F429_RTC.hpp"
#else
#include "host_rtc.hpp"
#endif
#include "rtcInterface.hpp"

class virtualRTC
{
  private:
#ifdef TARGET_MICRO
	stm429RTC loggerRTC;
#else
	simulatedRTC loggerRTC;
#endif

	rtcInterface* interface;

  public:
	virtualRTC() : interface(&loggerRTC) {}
	/**
     * @brief Sets the time.
     * 
     * @param hour Hour (0-23).
     * @param minute Minute (0-59).
     * @param seconds Seconds (0-59).
     * @return true if the time is valid and set successfully.
     * @return false if any parameter is out of range.
     */
	bool setTime(uint8_t hour, uint8_t minute, uint8_t seconds)
	{
		return this->interface->setTime(hour, minute, seconds);
	}

	/**
     * @brief Retrieves the formatted time.
     * 
     * Writes the formatted time ("HH:MM:SS") into the provided buffer.
     * The buffer must be at least 9 bytes long (including the null terminator).
     * 
     * @param buffer Pointer to the character array to store the formatted time.
     * @param bufferSize Size of the provided buffer.
     */
	void getTime(char* buffer, size_t bufferSize) const
	{
		this->interface->getTime(buffer, bufferSize);
	}

	/**
     * @brief Sets the date.
     * 
     * @param day Day (1-31).
     * @param month Month (1-12).
     * @param year Year (e.g. 2025).
     * @return true if the date is valid and set successfully.
     * @return false if any parameter is out of range.
     */
	bool setDate(uint8_t day, uint8_t month, uint16_t year)
	{
		return this->interface->setDate(day, month, year);
	}

	/**
     * @brief Retrieves the formatted date.
     * 
     * Writes the formatted date ("DD/MM/YYYY") into the provided buffer.
     * The buffer must be at least 11 bytes long (including the null terminator).
     * 
     * @param buffer Pointer to the character array to store the formatted date.
     * @param bufferSize Size of the provided buffer.
     */
	void getDate(char* buffer, size_t bufferSize) const
	{
		this->interface->getDate(buffer, bufferSize);
	}
};