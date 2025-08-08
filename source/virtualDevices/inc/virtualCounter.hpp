/**
 * This class abstracts a counter, 
 * 
 * Counter is increased from a ISR (target build)
 * or Shared memory (host build)
 * 
 * This class then offers an interface to read this resource
 * without worring about the type of build and also having a
 * barrier with the hardware in case of a change in the type
 * of the microcontroller
 * 
 */
#pragma once

#include <cstdint>

namespace virtualDevice
{
class virtualCounter
{
  public:
	/**
     * @brief This function reads the counter object and then
     * resets it to 0. 
     */
	uint16_t readandResetCounter();

  private:
	void resetCounter();
};
} // namespace virtualDevice