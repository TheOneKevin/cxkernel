/**
 * @file   page_alloc.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Sunday, November 26th 2017, 9:42:00 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-07-05T20:44:44-04:00
 */

#pragma once

#include "common.h"
#include "arch/arch_types.h"

#define PALLOC_GET_IDX(index, order) ((index) >> (1 + (order)))

#ifdef __cplusplus
extern "C" {
#endif

void pmm_init(void);
bool pmm_update_all(void);
phys_t pmm_alloc_page(void);
void pmm_free_page(phys_t);
void pmm_free_page_mult(phys_t);

#ifdef __cplusplus
}
#endif
