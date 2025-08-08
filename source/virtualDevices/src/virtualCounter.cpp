#include "virtualCounter.hpp"
#include "sensorSimulatorConsumer.hpp"

namespace virtualDevice
{
uint16_t virtualCounter::readandResetCounter()
{
#ifndef TARGET_MICRO
	sensorSimulator::sensorOutput output = sensorSimulator::readSharedMemory();
	return output.rain;
#endif
	return 0;
}

void virtualCounter::resetCounter()
{
#ifndef TARGET_MICRO
#endif
}
} // namespace virtualDevice