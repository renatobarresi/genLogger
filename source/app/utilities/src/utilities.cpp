#include "utilities.hpp"
#include <cstdint>
#include <cstdio>
#include <cstring>

#ifndef TARGET_MICRO
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unistd.h>
#endif

namespace utilities
{
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

#ifndef TARGET_MICRO
// TODO, even though this is only used when build is for host,
// This can be calculated at compile time using source_location
std::string getPathMetadata(std::string fileName)
{
	char	buf[4096];
	ssize_t len = readlink("/proc/self/exe", buf, sizeof(buf) - 1);
	if (len == -1)
	{
		throw std::runtime_error("Unable to get executable path");
	}
	buf[len] = '\0';

	std::filesystem::path execDir = std::filesystem::path(buf).parent_path();

	// Convert to string to manipulate
	std::string execDirStr = execDir.string();
	size_t		pos		   = execDirStr.find("/build");
	if (pos != std::string::npos)
	{
		execDirStr.erase(pos);
	}

	// Assign back to path and append metadata path
	execDir = std::filesystem::path(execDirStr);
	execDir /= "test/simulationFiles";
	execDir /= fileName;

	std::cout << "Path to metadata: " << execDir << std::endl;

	return execDir.string();
}

std::string getLastLine(std::string fileName)
{
	std::ifstream file(fileName);
	std::string	  line;
	std::string	  lastLine;

	if (!file.is_open())
	{
		std::cout << "Unable to open file for reading: " << fileName << std::endl;
		return "";
	}

	while (std::getline(file, line))
	{
		// Trim trailing '\r' if it exists (for Windows/DOS line endings)
		if (!line.empty() && line.back() == '\r')
		{
			line.pop_back();
		}

		// We are interested in the last non-empty line
		if (!line.empty() && line != "")
		{
			lastLine = line + "\n";
		}
	}

	return lastLine;
}
#endif
} // namespace utilities