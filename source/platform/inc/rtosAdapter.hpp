#pragma once

class rtosAdapter
{
  public:
	virtual bool threadCreate(const char* threadName, void* (*startRoutine)(void*), int stackSize,
							  int priority, void* args) = 0;
	virtual bool startScheduler() = 0;
	virtual ~rtosAdapter() {}
};