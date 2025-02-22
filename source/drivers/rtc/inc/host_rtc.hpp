#pragma once

#include "rtcInterface.hpp"
#include <cstddef>
#include <cstdint>
#include <ctime>

/**
 * @brief Simulated Real-Time Clock (RTC) for embedded systems.
 * 
 * This class implements the virtualRTCInterface using the system clock as a basis.
 * It provides functions to set and get time and date without dynamic memory allocation.
 */
class simulatedRTC : public rtcInterface
{
  public:
	/**
     * @brief Constructs a new simulatedRTC object.
     * 
     * The RTC is initialized with the current system time.
     */
	simulatedRTC();

	bool init() override;

	/**
     * @brief Sets the time.
     * 
     * @param hour Hour (0-23).
     * @param minute Minute (0-59).
     * @param seconds Seconds (0-59).
     * @return true if the time is valid and set successfully.
     * @return false if any parameter is out of range.
     */
	bool setTime(uint8_t hour, uint8_t minute, uint8_t seconds) override;

	/**
     * @brief Retrieves the formatted time.
     * 
     * Writes the formatted time ("HH:MM:SS") into the provided buffer.
     * The buffer must be at least 9 bytes long (including the null terminator).
     * 
     * @param buffer Pointer to the character array to store the formatted time.
     * @param bufferSize Size of the provided buffer.
     */
	void getTime(char* buffer, size_t bufferSize) const override;

	/**
     * @brief Sets the date.
     * 
     * @param day Day (1-31).
     * @param month Month (1-12).
     * @param year Year (e.g. 2025).
     * @return true if the date is valid and set successfully.
     * @return false if any parameter is out of range.
     */
	bool setDate(uint8_t day, uint8_t month, uint16_t year) override;

	/**
     * @brief Retrieves the formatted date.
     * 
     * Writes the formatted date ("DD/MM/YYYY") into the provided buffer.
     * The buffer must be at least 11 bytes long (including the null terminator).
     * 
     * @param buffer Pointer to the character array to store the formatted date.
     * @param bufferSize Size of the provided buffer.
     */
	void getDate(char* buffer, size_t bufferSize) const override;

  private:
	/// Stores the current time and date.
	std::tm currentTime;

	/**
     * @brief Synchronizes the current time structure.
     * 
     * Uses std::mktime to normalize the tm structure. This takes care of any
     * overflows in the time values (e.g., 60 seconds rolling over to the next minute).
     */
	void syncSystemTime();
};
