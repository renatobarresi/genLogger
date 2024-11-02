#pragma once

class LED
{
    public:
    typedef enum
    {
        ledOne,
        ledTwo,
        ledThree
    } leds_t;

    void blinkLed(leds_t led, int delay);
};
