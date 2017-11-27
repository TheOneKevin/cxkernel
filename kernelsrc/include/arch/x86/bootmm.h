/*
 * File:   bootmm.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 2017-10-02T16:46:55-04:00
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2017-11-26T20:48:52-05:00
*/

#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

void bootmm_init_memory_regions(void);
void bootmm_free_page(unsigned int address);
void* bootmm_alloc_page(void);

#ifdef __cplusplus
}
#endif
