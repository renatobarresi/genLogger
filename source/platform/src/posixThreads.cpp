#include "posixThreads.hpp"
#include <pthread.h>

bool posixThreads::threadCreate(const char* threadName, void* (*startRoutine)(void*), int stackSize,
								int priority, void* args)
{
	return true;
}

bool posixThreads::startScheduler()
{
	return true;
}