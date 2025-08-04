#pragma once

#include <cstddef>
#include <cstdint>

namespace sensorSimulator
{
struct sensorOutput 
{
    uint32_t rain;
    uint32_t windSpeed;
    uint32_t windDir;
};

// union sharedMemory
// {
//     sensorOutput data;
//     std::byte sensorOutputBuff[sizeof(sensorOutput)];
// };

void init(void);

sensorOutput readSharedMemory(void);
}