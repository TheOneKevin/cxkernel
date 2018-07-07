/**
 * @file
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Sunday, April 1st 2018, 5:54:52 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-07-05T22:29:51-04:00
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
virt_t (*arch_get_virt)(phys_t);
phys_t (*arch_get_phys)(virt_t);

#ifdef __cplusplus
}
#endif
