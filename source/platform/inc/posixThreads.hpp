#pragma once

#include "rtosAdapter.hpp"
#include "rtosFactory.hpp"

class posixThreads : public rtosAdapter
{
  public:
	bool threadCreate(const char* threadName, void* (*startRoutine)(void*), int stackSize, int priority, void* args) override;
	bool startScheduler() override;
	void delay(int delay_ms) override;
};

class posixThreadsFactory : public rtosFactory
{
  public:
	rtosAdapter* factory() override
	{
		return new posixThreads;
	}
};