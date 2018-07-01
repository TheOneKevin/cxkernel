/*
 * File:   mmtypes.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 2018-04-01T15:17:20-04:00
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2018-05-23T13:41:59-04:00
*/

#pragma once

#include "common.h"
#include "list.h"
#include "bitmap.h"
#include "arch/atomic.h"

#define MAX_NR_ORDER 10
#define MAX_NR_ZONES 3

// 32 bytes, can fit 128 per page (4096 bytes in a page, so 128 * 32 = 4096)
typedef struct frame
{
    list_head_t list;
    atomic_t ref_count;
    unsigned long idx;
    uint32_t flags;
    uint64_t priv;
    void* virt;
} mem_map_t;

struct free_list
{
    list_head_t list;
    bitmap_t* map;
};

typedef struct buddy_ctx
{
    unsigned int order_bit;
    unsigned int order_max;
    struct free_list *free_area;  // One free area per order
    struct frame     *frames;                   // Pointer to g_mmap
} buddy_ctx_t;
