/**
 * @file
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Saturday, June 30th 2018, 6:23:18 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-07-06T07:45:38-04:00
 */

#pragma once

#include "common.h"
#include "arch/arch_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Maps a single page from given physical address to
 * given virtual address. Will overwrite any existing mappings.
 * @param va Virtual address to map to
 * @param pa Physical address to map
 * @param f  Flags
 * @return   0 on success
 */
int map_single_page(virt_t va, phys_t pa, int f);

/**
 * Will return the physical address mapped
 * at the given virtual address.
 * @param va Virtual address
 * @return   Physical address on success, -1 on error (see errno)
 */
phys_t get_phys_addr(virt_t va);

#ifdef __cplusplus
}
#endif