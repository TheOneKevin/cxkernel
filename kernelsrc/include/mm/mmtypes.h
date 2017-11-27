/*
 * File:   mmtypes.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 2017-08-30T19:46:57-04:00
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2017-11-26T11:03:05-05:00
*/

#pragma once

#include "list.h"
#include "common.h"
#include "arch/atomic.h"

#ifdef __cplusplus
extern "C" {
#endif

// 32 bytes, can fit 128 per page (4096 bytes)
typedef struct page
{
    list_head_t list;
    atomic_t ref_count;
    unsigned long idx;
    uint32_t flags;
    uint64_t priv;
    void* virt;
} mem_map_t;

typedef struct
{
    list_head_t free_list;
} free_area_t;

#ifdef __cplusplus
}
#endif
