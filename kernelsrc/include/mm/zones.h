/*
 * File:   zones.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 2017-09-22T18:21:39-04:00
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2017-10-07T17:06:35-04:00
*/

#pragma once

#include "common.h"
#include "mm/mmtypes.h"

#define MAX_NR_ORDER 10
#define MAX_NR_ZONES 3

#ifdef __cplusplus
extern "C" {
#endif

typedef struct zone
{
    free_area_t free_area[MAX_NR_ORDER];
} zone_t;

typedef struct
{
    struct zone zones[MAX_NR_ZONES];
} pg_data_t;

#ifdef __cplusplus
}
#endif
