#ifdef TARGET_MICRO

#include "mongoose.h"
#include "rand.h"
#include "timer.h"
#include <string.h>

bool mg_random(void* buf, size_t len)
{
	for (size_t n = 0; n < len; n += sizeof(uint32_t))
	{
		uint32_t r;
		rand_generate_random_number(&r);
		memcpy((char*)buf + n, &r, n + sizeof(r) > len ? len - n : sizeof(r));
	}

	return true;
}

uint64_t mg_millis(void)
{
	return timer_getTick();
}

#endif