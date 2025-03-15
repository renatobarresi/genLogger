#include "errorHandler.hpp"

void appAssert(bool condition)
{
	if (!condition)
	{
		while (1);
	}
}