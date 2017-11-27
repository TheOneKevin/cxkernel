/*
 * File:   page_alloc.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 2017-08-30T16:36:48-04:00
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2017-11-26T12:28:32-05:00
*/

#pragma once

#include "common.h"
#include "bitmap.h"
#include "mm/mmtypes.h"

mem_map_t* global_mmap_root;

#define PALLOC_GET_IDX(index, order) ((index) >> (1 + (order)))

#ifdef __cplusplus
extern "C" {
#endif

void pmm_init(void);

#ifdef __cplusplus
}
#endif
