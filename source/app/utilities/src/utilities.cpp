#include "utilities.hpp"
#include <cstdint>
#include <cstdio>
#include <cstring>

bool parseTimeAndDate(const char* buff, int* hour, int* minute, int* seconds, int* day, int* month, int* year)
{
	// Check for null input
	if (buff == nullptr || strlen(buff) == 0)
	{
		return false;
	}

	// Try to parse using both possible formats
	int convertedChars = sscanf(buff, "%2d:%2d:%2d %2d/%2d/%4d", hour, minute, seconds, day, month, year);

	// If both formats failed, return false
	if (convertedChars != 6)
	{
		return false;
	}

	// Validate parsed values
	if (*hour > 23 || *minute > 59 || *seconds > 59 || *day == 0 || *day > 31 || *month == 0 || *month > 12 || *year > 9999)
	{
		return false;
	}

	return true;
}
