#include "stm32f429zi_init.hpp"
#include "ads1115.hpp"

int main()
{
    // Init ADS1115
    ADS1115 myADS1115; 

    // Init BSP
    stm32f429_init();
    
    return 0;
}