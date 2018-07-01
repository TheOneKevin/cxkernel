/*
 * File:   zones.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 2017-09-22T18:21:39-04:00
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2018-04-01T20:31:20-04:00
*/

#pragma once

#include "common.h"
#include "bitmap.h"
#include "mm/mmtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    list_head_t free_list;
} free_area_t;

typedef struct zone
{
    bitmap_t* free_bmp[1];
} zone_t;

typedef struct
{
    struct zone zones[1];
} pg_data_t;

#ifdef __cplusplus
}
#endif
