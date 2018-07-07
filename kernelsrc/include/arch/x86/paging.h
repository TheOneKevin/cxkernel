/**
 * @file
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Sunday, November 26th 2017, 9:42:00 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-07-06T07:46:31-04:00
 */

#pragma once

#include "common.h"
#include "arch/arch_paging.h"
#include "arch/x86/arch_common.h"

/*
 * (PAE SCHEME)
 * 0x004 PD in PDPT
 * 0x200 entries per PD
 * 0x200 entries per PT
 *
 * So 0x1FF entires
 * So 0x200 * 0x      1000 = 0x     20 0000 bytes mapped per PT (0b0010 0000 0000 0000 0000 0000, 21 zeros)
 * So 0x200 * 0x   20 0000 = 0x   4000 0000 bytes mapped per PD (0b0100 0000 0000 0000 0000 0000 0000 0000, 30 zeros)
 * So 0x004 * 0x 4000 0000 = 0x 1 0000 0000 bytes mapped per PDPT
 *
 * (NON PAE SCHEME)
 * 0x001 PD in PDPT (no PDPT)
 * 0x400 entries per PD
 * 0x400 entries per PT
 *
 * So 0x3FF entries
 * So 0x400 * 0x    1000 = 0x     40 0000 bytes mapped per PT (0b0100 0000 0000 0000 0000 0000, 22 zeros)
 * So 0x400 * 0x 40 0000 = 0x 1 0000 0000 bytes mapped per PD
*/

#define ARCH_GET_PD_IDX(a) ((a) >> 22 & 0x03FF)
#define ARCH_GET_PT_IDX(a) ((a) >> 12 & 0x03FF)

#define ARCH_PAE_GET_PDPT_IDX(a) ((a) >> 30 & 0x01FF)
#define ARCH_PAE_GET_PD_IDX(a)   ((a) >> 21 & 0x01FF)
#define ARCH_PAE_GET_PT_IDX(a)   ((a) >> 12 & 0x01FF)

#define PDP_PR 1 << 0

#define PDE_PR  1 << 0
#define PDE_RW  1 << 1
#define PDE_US  1 << 2
#define PDE_PWT 1 << 3
#define PDE_PCD 1 << 4

#define PTE_PR 1 << 0
#define PTE_RW 1 << 1
#define PTE_GL 1 << 8

#ifdef __cplusplus
extern "C" {
#endif

void init_paging(void);

#ifdef __cplusplus
}
#endif
