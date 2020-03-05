/**
 * Copyright (c) 2019 The cxkernel Authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 * 
 * @file   pmm.cc
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on July 16 2019, 10:34 AM
 */

#include <assert.h>
#include "core/memory.h"

namespace pmm
{
    static PhysicalAllocator* allocator = NULL;

    void set_allocator(PhysicalAllocator* alloc)
    {
        allocator = alloc;
    }

    PhysicalAllocator& get_allocator()
    {
        return static_cast<PhysicalAllocator&>(*allocator);
    }

    void add_arena(pmm_arena_t* arena, bitmap_t* bt)
    {
        get_allocator().AddArena(arena, bt);
    }

    size_t alloc_pages(size_t cnt, list_head_t** pages)
    {
        ASSERT(get_allocator().GetType() == PMM_TYPE_LIST, "Are you using the wrong allocator?");
        return get_allocator().Allocate(cnt, (uintptr_t) pages);
    }

    size_t alloc_pages_contig(size_t cnt, list_head_t** pages)
    {
        ASSERT(get_allocator().GetType() == PMM_TYPE_LIST, "Are you using the wrong allocator?");
        return get_allocator().AllocateContiguous(cnt, (uintptr_t) pages);
    }

    size_t free(list_head_t** pages)
    {
        ASSERT(get_allocator().GetType() == PMM_TYPE_LIST, "Are you using the wrong allocator?");
        return get_allocator().Free((uintptr_t) pages);
    }

    phys_t get_phys(page_t* page)
    {
        ASSERT(get_allocator().GetType() == PMM_TYPE_LIST, "Are you using the wrong allocator?");
        return get_allocator().PageToPhysical((uintptr_t) page);
    }

    #pragma GCC diagnostic ignored "-Wunused-parameter"
    
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
        return sizeof(list_node_t*);
    }
}