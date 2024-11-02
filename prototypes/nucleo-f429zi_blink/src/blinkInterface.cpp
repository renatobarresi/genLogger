#include "blinkInterface.hpp"
#include "leds.h"

void LED::blinkLed(leds_t led, int delay)
{
    turnOnRedLed();
    ledDelay(delay);
    turnOffRedLed();
}