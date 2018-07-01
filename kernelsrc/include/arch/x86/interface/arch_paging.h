/*
 * File:   arch_paging.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 2018-04-01T17:54:52-04:00
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2018-04-01T20:22:35-04:00
*/

#pragma once

#include "common.h"
#include "arch/x86/arch_common.h"
#include "arch/arch_types.h"

#define ARCH_PAGE_MASK (~(ARCH_PAGE_SIZE - 1))
#define ARCH_PAGE_MASK_LL ((~(0x1000ULL - 1ULL)))
#define ARCH_PAGE_ALIGN(addr) (((addr) + ARCH_PAGE_SIZE - 1) & ARCH_PAGE_MASK)
#define ARCH_PAGE_ALIGN_DOWN(n) (ARCH_PAGE_ALIGN(n) == n ? ARCH_PAGE_ALIGN(n) : ARCH_PAGE_ALIGN(n) - ARCH_PAGE_SIZE)
#define ARCH_PAGE_SIZE 0x1000

#ifdef __cplusplus
extern "C" {
#endif

int (*arch_map_page)(virt_t, phys_t, uint16_t);
virt_t (*arch_get_virt)(ON_ERR(), phys_t);
phys_t (*arch_get_phys)(ON_ERR(), virt_t);

#ifdef __cplusplus
}
#endif
