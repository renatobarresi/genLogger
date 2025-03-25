#pragma once

#include <cstdint>

bool parseTimeAndDate(const char* buff, int* hour, int* minute, int* seconds, int* day, int* month, int* year);