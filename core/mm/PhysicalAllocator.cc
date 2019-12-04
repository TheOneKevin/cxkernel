/**
 * Copyright (c) 2019 The cxkernel Authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 *
 * @file   pmm.cc
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on March 18 2019, 11:15 PM
 */

#define __MODULE__ "PMM"

#include <stdlib.h>
#include <linked_list.h>
#include <panic.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "arch/x86/arch_utils.h"
#include "system.h"
#include "core/vm.h"
#include "arch/interface.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

namespace pmm
{
    static PhysicalAllocator* allocator = NULL;

    void SetPhysicalAllocator(PhysicalAllocator* alloc)
    {
        allocator = alloc;
    }

    PhysicalAllocator& GetPhysicalAllocator()
    {
        return static_cast<PhysicalAllocator&>(*allocator);
    }

    void PhysicalAllocator::AddArena(pmm_arena_t* arena, bitmap_t* bt)
    {
        // Do nothing
    }

    phys_t PhysicalAllocator::PageToPhysical(uintptr_t page)
    {
        return -1;
    }

    size_t PhysicalAllocator::Allocate(size_t sz, uintptr_t pages)
    {
        return 0;
    }

    size_t PhysicalAllocator::AllocateSingle(uintptr_t pages)
    {
        return Allocate(1, pages);
    }

    size_t PhysicalAllocator::AllocateContiguous(size_t sz, uintptr_t pages)
    {
        return 0;
    }

    size_t PhysicalAllocator::Free(uintptr_t pages)
    {
        return 0;
    }

    int PhysicalAllocator::GetType()
    {
        return PMM_TYPE_OTHER;
    }

    size_t PhysicalAllocator::GetSize()
    {
        return sizeof(list_node_t); // Note without the pointer
    }
}
