/**
 * Copyright (c) 2018 The cxkernel Authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 *
 * @file   arch_common.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on December 24 2018, 11:25 PM
 */

#pragma once

#define ARCH_PAGE_SHIFT     12
#define ARCH_PAGE_SIZE      (1 << (ARCH_PAGE_SHIFT))
#define ARCH_PAGE_MASK      (~(ARCH_PAGE_SIZE - 1))
#define ARCH_VIRT_BASE      0xE0000000
#define ARCH_STACK_BOTTOM   0xFF800000
#define ARCH_KMALLOC_LENGTH 0x20000000
#define ARCH_KMALLOC_BASE   0xC0000000
