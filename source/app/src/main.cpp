#include "posixThreads.hpp"
#include "rtosFactory.hpp"
#include <stdio.h>

void* commManagerThread(void* args)
{
	printf("Starting from comm thread\r\n");
	while(1)
	{
		printf("Running comm thread\r\n");
	}
}

void* sensorManagerThread(void* args)
{
	printf("Starting from sensor thread\r\n");
	while(1)
	{
		printf("Running sensor thread\r\n");
	}
}

int main()
{
	rtosFactory* myFactory = new posixThreadsFactory();
	rtosAdapter* myRTOS = myFactory->factory();

	myRTOS->threadCreate("test thread", &commManagerThread, 1024, 0, nullptr);
	myRTOS->startScheduler();

	delete myRTOS;
	delete myFactory;

	return 0;
}