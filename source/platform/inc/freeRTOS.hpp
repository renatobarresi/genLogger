#pragma once

#include "rtosAdapter.hpp"

class freeRTOS : public rtosAdapter
{
  public:
	bool hreadCreate(const char& threadName, void* (*startRoutine)(void*), int stackSize, int priority, void* args) override;
	bool startScheduler() override;
};