/*
 * File:   paging.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 2017-10-09T20:16:25-04:00
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2017-10-10T14:42:42-04:00
*/

#pragma once

#include "common.h"

#define ARCH_GET_PD_NUMBER(a) ((a) >> 22)
#define ARCH_GET_PT_NUMBER(a) ((a) >> 12 & 0x03FF)

#ifdef __cplusplus
extern "C" {
#endif

uint64_t g_paging_PDPT[4] ALIGNED(0x20);

#ifdef __cplusplus
}
#endif
