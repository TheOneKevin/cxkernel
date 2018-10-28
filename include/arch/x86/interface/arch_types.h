/**
 * @file   arch_types.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 *
 * @date Created on Saturday, October 13th 2018, 5:53:26 pm
 *
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-10-27T19:50:19-04:00
 */

#pragma once
#include "common.h"

#if ARCH_TYPE == ARCH_x86_32
    typedef uint32_t arch_sz_t;
    typedef uint32_t virt_t;
    typedef uint64_t phys_t;
#elif ARCH_TYPE == ARCH_x86_64
    typedef uint64_t arch_sz_t;
    typedef uint64_t virt_t;
    typedef uint64_t phys_t;
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif
