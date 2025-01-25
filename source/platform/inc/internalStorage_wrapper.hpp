/**
 * @file internalStorage.hpp
 * @author
 * @brief Provides an interface for file operations compatible with both microcontroller and host
 * platforms.
 * @version 0.1
 * @date 2025-01-24
 *
 * @copyright Copyright (c) 2025
 */

#pragma once

#include <cstddef>
#include <cstdint>

/**
 * @brief Opens a file for reading, writing, or appending.
 *
 * @param fileName The name of the file to open.
 * @param mode The mode to open the file in:
 *             - 0: Read mode
 *             - 1: Write mode
 *             - 2: Append mode
 * @return int8_t Returns 0 on success, -1 on failure.
 */
int8_t open(const char* fileName, uint8_t mode);

/**
 * @brief Writes data to the currently open file.
 *
 * @param buffer Pointer to the data to be written.
 * @param size Number of bytes to write.
 * @return int32_t Number of bytes written, or -1 on failure.
 */
int32_t write(const char* buffer, size_t size);

/**
 * @brief Reads data from the currently open file.
 *
 * @param buffer Pointer to the buffer where data will be stored.
 * @param size Number of bytes to read.
 * @return int32_t Number of bytes read, or -1 on failure.
 */
int32_t read(char* buffer, size_t size);

/**
 * @brief Closes the currently open file.
 *
 * @return int8_t Returns 0 on success, -1 on failure.
 */
int8_t close();
