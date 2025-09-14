/**
 * @file utilities.hpp
 * @author renato (renatobarresi@gmail.com)
 * @brief generic functions 
 * @version 0.1
 * @date 2025-03-27
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once

#include <cstdint>

#ifndef TARGET_MICRO
#include <string>
#endif

namespace utilities
{

constexpr uint16_t MS_IN_ONE_MINUTE = 60000;

/**
 * @brief Parses a string and sets time and date values according to what's parsed
 *
 * This function uses the c standard library cal sscanf to do the string parsing, 
 * Also checks for null pointers or 0 lenght string, if the input string is invalid
 * it returns false. 
 * 
 * @param[in] buff 
 * @param[out] hour 
 * @param[out] minute 
 * @param[out] seconds 
 * @param[out] day 
 * @param[out] month 
 * @param[out] year 
 * 
 * @return true operation was successful 
 * @return false wrong format
 */
bool parseTimeAndDate(const char* buff, int* hour, int* minute, int* seconds, int* day, int* month, int* year);

#ifndef TARGET_MICRO
std::string getPathMetadata(std::string fileName);

std::string getLastLine(std::string fileName);
#endif
}