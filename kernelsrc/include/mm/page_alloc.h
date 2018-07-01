/*
 * File:   page_alloc.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 2017-08-30T16:36:48-04:00
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2018-04-01T15:27:30-04:00
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
