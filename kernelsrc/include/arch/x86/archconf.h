/**
 * @file   archconf.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Monday, March 19th 2018, 2:10:51 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-07-05T20:38:33-04:00
 */

#pragma once

#define PAE_ENABLE  1
#define SELF_TEST   1

#if PAE_ENABLE == 1
    #define ARCH_PAE_ENABLE
#endif

#if KERNEL_SELF_TEST == 1
    #define KERNEL_SELF_TEST
#endif

#undef PAE_ENABLE
#undef SELF_TEST
