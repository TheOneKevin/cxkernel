/*
 * File:   time.h
 * Author: kevin
 *
 * Created on December 20, 2016, 6:49 AM
 */

#include <stddef.h>
#include <stdint.h>

#ifndef TIME_H
#define TIME_H

#ifdef __cplusplus
extern "C" {
#endif

struct date_time
{
    uint8_t  day;
    uint8_t  month;
    uint32_t year;

    uint8_t  hour;
    uint16_t minutes;
    uint16_t seconds;
}; typedef struct date_time datetime_t;

extern unsigned char second;
extern unsigned char minute;
extern unsigned char hour;
extern unsigned char day;
extern unsigned char month;
extern unsigned int  year;

void read_rtc ();
uint64_t getEpoch ();

#ifdef __cplusplus
}
#endif

#endif /* TIME_H */
