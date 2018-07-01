/*
 * File:   archconf.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 2018-03-19T14:10:51-04:00
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2018-03-28T07:56:18-04:00
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
