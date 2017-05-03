/* 
 * File:   time.h
 * Author: kevin
 *
 * Created on December 20, 2016, 6:49 AM
 */

#include "common.h"

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

status_t __secs_to_tm(long long t, datetime_t *tm);

#ifdef __cplusplus
}
#endif

#endif /* TIME_H */
