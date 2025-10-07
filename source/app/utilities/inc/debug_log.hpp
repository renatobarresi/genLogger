#pragma once

#include <cstdarg>
#include <cstdio>
#include <source_location>
#include <virtualRTC.hpp>

namespace debug
{
enum class logLevel
{
	LOG_WARNING,
	LOG_ERROR,
	LOG_ALL,
};

inline constexpr logLevel systemLogLevel = logLevel::LOG_ALL;

// find the base name of a file path.
constexpr const char* file_basename(const char* path)
{
	const char* base = path;
	while (*path)
	{
		if (*path == '/' || *path == '\\')
		{
			base = path + 1;
		}
		path++;
	}
	return base;
}

template<bool E, logLevel L>
inline void log(const char* fmt, ...)
{
	if constexpr (E)
	{
		if (L <= systemLogLevel)
		{
			va_list args;
			va_start(args, fmt);
			vprintf(fmt, args);
			va_end(args);
		}
	}
}
} // namespace debug