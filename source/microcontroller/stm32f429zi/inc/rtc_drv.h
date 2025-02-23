#ifndef LEDS_H
#define LEDS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int8_t rct_init(void);

int8_t rtc_set_time(uint8_t hour, uint8_t minute, uint8_t second);

int8_t rtc_set_date(uint8_t year, uint8_t month, uint8_t day);

void rtc_get_date(uint8_t* year, uint8_t* month, uint8_t* day);

#ifdef __cplusplus
}
#endif

#endif