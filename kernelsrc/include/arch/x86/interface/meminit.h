/*
 * File:   meminit.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 2017-08-25T20:00:54-04:00
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2017-11-26T12:47:37-05:00
*/

#pragma once

#include "common.h"
#include "bitmap.h"

#ifdef __cplusplus
extern "C" {
#endif

void arch_pmeminit(void);

// Global bitmap used for the buddy page allocator
bitmap_t* g_pmm_buddy_map;

#ifdef __cplusplus
}
#endif
