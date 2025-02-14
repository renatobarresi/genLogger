#pragma once

#include <cstddef>
#include <cstdint>

struct rtcInterface
{
	// Set time. Returns false if any parameter is out of range.
	virtual bool setTime(uint8_t hour, uint8_t minute, uint8_t seconds) = 0;
	// Write formatted time ("HH:MM:SS") into buffer (which must be at least 9 bytes).
	virtual void getTime(char* buffer, size_t bufferSize) const = 0;

	// Set date. Returns false if any parameter is out of range.
	virtual bool setDate(uint8_t day, uint8_t month, uint16_t year) = 0;
	// Write formatted date ("DD/MM/YYYY") into buffer (which must be at least 11 bytes).
	virtual void getDate(char* buffer, size_t bufferSize) const = 0;

	virtual ~rtcInterface() = default;
};
