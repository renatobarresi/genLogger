#include "posixThreads.hpp"
#include "rtosFactory.hpp"

void* foo(void* args)
{
	while(1)
	{
	}
}

int main()
{
	rtosFactory* myFactory = new posixThreadsFactory();
	rtosAdapter* myRTOS = myFactory->factory();

	myRTOS->threadCreate("test thread", &foo, 1024, 0, nullptr);
	myRTOS->startScheduler();

	delete myRTOS;
	delete myFactory;

	return 0;
}