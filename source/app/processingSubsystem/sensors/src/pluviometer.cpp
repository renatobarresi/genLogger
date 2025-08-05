#include "pluviometer.hpp"

namespace sensor
{   
    bool davisPluviometer::init()
    {
        // Init shared memory
        
        // Set GPIO as input

        // Set GPIO interrupt 

        return true;

    }

    uint16_t davisPluviometer::getRain()
    {
        uint16_t rain;
        // Get GPIO counter

        // Reset GPIO Counter

        return rain;
    }
}