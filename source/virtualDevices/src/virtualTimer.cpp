#pragma once

#include "virtualTimer.hpp"

namespace systick
{

#ifndef TARGET_MICRO
// Internal static instance for simulation
static Timer1msSimulator	 _simulator;
static std::atomic<uint64_t> tickCount = 0;
struct hardwareTimeouts*	 pTask_1;

// Externally callable function to start simulation
void startSystickSimulation(struct hardwareTimeouts** taskControlParams)
{
	pTask_1 = taskControlParams[0];
	_simulator.start(myTickHandler);
}

// Optional stop function
void stopSystickSimulation()
{
	_simulator.stop();
}

void myTickHandler()
{
	tickCount++;
	if ((tickCount % *pTask_1->taskTimeout) == 0)
	{
		*pTask_1->taskRunFlag = 1;
	}
}
#endif
uint64_t getTicks()
{
	uint64_t ticks;
#ifndef TARGET_MICRO
	ticks = tickCount.load(std::memory_order_relaxed);
#else
	ticks = timer_getTick();
#endif
	return ticks;
}
} // namespace systick