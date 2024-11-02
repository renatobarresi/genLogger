#include "helloWorld.h"
// #include <nuttx/config.h>

extern "C" {
int hellocpp_main(void)
{
	CHelloWorld* pHelloWorld = new CHelloWorld();
	pHelloWorld->HelloWorld();

	CHelloWorld helloWorld;
	helloWorld.HelloWorld();

	delete pHelloWorld;
	return 0;
}
}