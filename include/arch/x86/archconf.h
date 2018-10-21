/**
 * @file   archconf.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Monday, March 19th 2018, 2:10:51 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-10-14T09:09:53-04:00
 */

#pragma once

#define PAE_ENABLE  1
#define SELF_TEST   1
#define x86_32
#define x86_64

#if PAE_ENABLE == 1
    #define ARCH_PAE_ENABLE
#endif

#if KERNEL_SELF_TEST == 1
    #define KERNEL_SELF_TEST
#endif

// Disable these shites
#undef PAE_ENABLE
#undef SELF_TEST
#undef x86_64
