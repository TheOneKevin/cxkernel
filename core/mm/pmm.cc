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

    void add_arena(arena_t* arena, bitmap_t* bt)
    {
        get_allocator().add_arena(arena, bt);
    }

    size_t alloc_pages(size_t cnt, list_head_t** pages)
    {
        ASSERT(get_allocator().get_type() == PMM_TYPE_LIST, "Are you using the wrong allocator?");
        return get_allocator().allocate(cnt, (uintptr_t) pages);
    }

    size_t alloc_pages_contig(size_t cnt, list_head_t** pages)
    {
        ASSERT(get_allocator().get_type() == PMM_TYPE_LIST, "Are you using the wrong allocator?");
        return get_allocator().allocate_contiguous(cnt, (uintptr_t) pages);
    }

    size_t free(list_head_t** pages)
    {
        ASSERT(get_allocator().get_type() == PMM_TYPE_LIST, "Are you using the wrong allocator?");
        return get_allocator().free((uintptr_t) pages);
    }

    phys_t get_phys(page_t* page)
    {
        ASSERT(get_allocator().get_type() == PMM_TYPE_LIST, "Are you using the wrong allocator?");
        return get_allocator().page_to_physical((uintptr_t) page);
    }

    #pragma GCC diagnostic ignored "-Wunused-parameter"
    
    void PhysicalAllocator::add_arena(arena_t* arena, bitmap_t* bt)
    {
        // Do nothing
    }

    phys_t PhysicalAllocator::page_to_physical(uintptr_t page)
    {
        return -1;
    }

    size_t PhysicalAllocator::allocate(size_t sz, uintptr_t pages)
    {
        return 0;
    }

    size_t PhysicalAllocator::allocate_single(uintptr_t pages)
    {
        return allocate(1, pages);
    }

    size_t PhysicalAllocator::allocate_contiguous(size_t sz, uintptr_t pages)
    {
        return 0;
    }

    size_t PhysicalAllocator::free(uintptr_t pages)
    {
        return 0;
    }

    int PhysicalAllocator::get_type()
    {
        return PMM_TYPE_OTHER;
    }

    size_t PhysicalAllocator::get_size()
    {
        return sizeof(list_node_t*);
    }
}