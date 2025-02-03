/**
 * @file leds.h
 * @author Renato Barresi (renatobarresi@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-02-03
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef LEDS_H
#define LEDS_H

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////
//							Function definition
////////////////////////////////////////////////////////////////////////

void turnOffRedLed();
void turnOnRedLed();
void ledDelay(int delay);

#ifdef __cplusplus
}
#endif

#endif