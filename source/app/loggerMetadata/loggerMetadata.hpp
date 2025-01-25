/**
 * @file loggerMetadata.hpp
 * @author Renato Barresi (renatobarresi@gmail.com)
 * @brief
 * @version 0.1
 * @date 2025-01-24
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#include <cstdint>

constexpr uint16_t loggerNameLenght = 96;

struct loggerMetadata
{
	char loggerName[loggerNameLenght];
};

struct loggerMetadata* getLoggerMetadata();