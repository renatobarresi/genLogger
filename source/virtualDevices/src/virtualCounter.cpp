#include "virtualCounter.hpp"
#include "sensorSimulatorConsumer.hpp"

namespace virtualDevice
{
uint16_t virtualCounter::readandResetCounter()
{
#ifndef TARGET_MICRO
	if (false == sensorSimulator::getMemInitFlag())
	{
		return 0;
	}
	sensorSimulator::sensorOutput output = sensorSimulator::readSharedMemory();
	return output.rain;
#endif
	//	Target implementation

	return 0;
}

void virtualCounter::resetCounter()
{
#ifndef TARGET_MICRO
#endif
}
} // namespace virtualDevice