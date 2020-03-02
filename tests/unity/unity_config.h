/**
 * Copyright (c) 2019 The cxkernel Authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 * 
 * @file   unity_config.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on December 27 2019, 4:56 PM
 */

#include "common.h"

#ifndef UNITY_CONFIG_H
#define UNITY_CONFIG_H

#define UNITY_INCLUDE_PRINT_FORMATTED

#if ARCH_TYPE == ARCH_x86_32 || ARCH_TYPE == ARCH_x86_64
    extern void __unityPutCharInternalPc(char);
    extern void __unityInitSerialPc();
    #define UNITY_OUTPUT_START() __unityInitSerialPc()
    #define UNITY_OUTPUT_CHAR(a) __unityPutCharInternalPc(a)

    #define UNITY_SUPPORT_64 // Both 32-bit and 64-bit x86 supports 64-bit operations
#endif

/* End of UNITY_CONFIG_H */
#endif