/**
 * Copyright (c) 2019 The cxkernel Authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 * 
 * @file   pmm.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on June 06 2019, 12:19 AM
 */

#pragma once

#include "common.h"

#define CX_ARENA_PRESENT            (1 << 0)
#define CX_ARENA_KERNEL             (1 << 1)

#define CX_ARENA_TYPE_NORMAL        0
#define CX_ARENA_TYPE_DMA32         1

#define CX_PAGE_OCCUPIED            (1 << 0)

#define CX_PERM_READ    			(1 << 0)
#define CX_PERM_WRITE   			(1 << 1)
#define CX_PERM_EXECUTE 			(1 << 2)
#define CX_PERM_MAP					(1 << 3)
#define CX_PERM_CLONE				(1 << 4)
#define CX_PERM_REASSIGN			(1 << 5)
#define CX_PERM_GET_PROPERY			(1 << 6)
#define CX_PERM_SET_PROPERY			(1 << 7)
#define CX_PERM_LOCK				(1 << 8)
#define CX_PERM_SPECIFIC			(1 << 9)
#define CX_PERM_SPECIFIC_OVERWRITE	(1 << 10)

// If we wish to use listed allocators and modify page lists
#define PMM_TYPE_LIST  1
#define PMM_TYPE_BOOT  2
#define PMM_TYPE_OTHER 3
