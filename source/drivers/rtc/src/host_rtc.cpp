#include "host_rtc.hpp"
#include <cstdio>
#include <ctime>

simulatedRTC::simulatedRTC()
{
	// Initialize with the current system time.
	std::time_t t = std::time(nullptr);
	currentTime	  = *std::localtime(&t);
}

bool simulatedRTC::init()
{
	return true;
}

bool simulatedRTC::setTime(uint8_t hour, uint8_t minute, uint8_t seconds)
{
	if (hour > 23 || minute > 59 || seconds > 59)
	{
		return false;
	}

	currentTime.tm_hour = hour;
	currentTime.tm_min	= minute;
	currentTime.tm_sec	= seconds;
	syncSystemTime();

	return true;
}

void simulatedRTC::getTime(char* buffer, size_t bufferSize) const
{
	// Ensure buffer is large enough for "HH:MM:SS" plus the null terminator.
	if (bufferSize < 9)
	{
		if (bufferSize > 0)
		{
			buffer[0] = '\0';
		}

		return;
	}

	std::snprintf(buffer, bufferSize, "%02d:%02d:%02d", currentTime.tm_hour, currentTime.tm_min, currentTime.tm_sec);
}

bool simulatedRTC::setDate(uint8_t day, uint8_t month, uint16_t year)
{
	// Minimal check for valid day and month.
	if (day < 1 || day > 31 || month < 1 || month > 12)
	{
		return false;
	}

	currentTime.tm_mday = day;
	currentTime.tm_mon	= month - 1;   // tm_mon is zero-based.
	currentTime.tm_year = year - 1900; // tm_year is years since 1900.

	syncSystemTime();

	return true;
}

void simulatedRTC::getDate(char* buffer, size_t bufferSize) const
{
	// Ensure buffer is large enough for "DD/MM/YYYY" plus the null terminator.
	if (bufferSize < 11)
	{
		if (bufferSize > 0)
		{
			buffer[0] = '\0';
		}
		return;
	}
	std::snprintf(buffer, bufferSize, "%02d/%02d/%04d", currentTime.tm_mday, currentTime.tm_mon + 1, currentTime.tm_year + 1900);
}

void simulatedRTC::syncSystemTime()
{
	std::mktime(&currentTime);
}
