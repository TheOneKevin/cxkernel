/*
 * File:   paging.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 * 
 * Created on Saturday, June 30th 2018, 6:23:18 pm
 * 
 * Last modified by:   Kevin Dai
 * Last modified time: 2018-06-30T18:43:59-04:00
 */

#pragma once

#include "common.h"
#include "arch/arch_types.h"

#ifdef __cplusplus
extern "C" {
#endif

int map_single_page(virt_t virt, phys_t phys, int flags);

#ifdef __cplusplus
}
#endif