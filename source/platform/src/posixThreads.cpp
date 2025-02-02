#include "posixThreads.hpp"
#include <pthread.h>
#include <stdio.h>

bool posixThreads::threadCreate(const char* threadName, void* (*startRoutine)(void*), int stackSize, int priority, void* args)
{
	printf("Creating thread");

	pthread_t thread;

	pthread_create(&thread, NULL, startRoutine, NULL);

	return true;
}

bool posixThreads::startScheduler()
{
	printf("Staring the scheduler");
	return true;
}

void posixThreads::delay(int delay_ms) {}