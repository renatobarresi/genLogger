#include "init.h"
#include "blinkInterface.hpp"

int main()
{

    stm32f429_init();

    LED redLed;
    
    while(true)
    {
        // Blink
        redLed.blinkLed(LED::ledOne, 100);
    }

    return 0;
}