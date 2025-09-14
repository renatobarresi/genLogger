#pragma once

#include <cstdio>
#include <iostream>
#include <source_location>

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
inline void log(const char* msg)
{
	if constexpr (E)
	{
		if (L <= systemLogLevel)
		{
			std::cout << msg;
		}
	}
}

// inline void log(const char* msg, logLevel level = logLevel::LOG_ALL, const std::source_location& location = std::source_location::current())
// {
// 	if (level <= systemLogLevel)
// 	{
// 		const char* filename = file_basename(location.file_name());
// #ifndef TARGET_MICRO
// 		if (logLevel::LOG_ERROR == systemLogLevel)
// 		{
// 			std::cout << "[" << filename << ":" << location.line() << "] " << msg << "\r\n";
// 		}
// 		else
// 		{
// 			std::cout << "[" << filename << "] " << msg << std::endl;
// 		}
// #else
// 		// TODO change std::cout to printf (in target printf should redirect to uart)
// 		// std::cout has a lot of overhead
// 		// You can also include the line number here with location.line()
// 		std::cout << "[" << location.file_name() << "] " << msg << std::endl;
// #endif
// 	}
// }

} // namespace debug