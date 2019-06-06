/*
 * Copyright (c) 2019 The cxkernel Authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 * 
 * @file   pmm.cc
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on June 05 2019, 9:03 PM
 */
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
    static PhysicalAllocator* allocator = nullptr;

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
        
    }

    size_t PhysicalAllocator::Allocate(size_t sz, allocpage_t pages)
    {
        return 0;
    }

    size_t PhysicalAllocator::AllocateSingle(allocpage_t pages)
    {
        return 0;
    }

    size_t PhysicalAllocator::Free(allocpage_t pages)
    {
        return 0;
    }

    int PhysicalAllocator::GetType()
    {
        return PMM_TYPE_LIST;
    }
}

void pmm_add_arena(pmm_arena_t* arena, bitmap_t* bt)
{
    pmm::GetPhysicalAllocator().AddArena(arena, bt);
}

size_t pmm_alloc_pages(size_t cnt, allocpage_t pages)
{
    return pmm::GetPhysicalAllocator().Allocate(cnt, pages);
}

size_t pmm_free(allocpage_t pages)
{
    return pmm::GetPhysicalAllocator().Free(pages);
}
