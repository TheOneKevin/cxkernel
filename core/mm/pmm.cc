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

void pmm_add_arena(pmm_arena_t* arena, bitmap_t* bt)
{
    pmm::GetPhysicalAllocator().AddArena(arena, bt);
}

size_t pmm_alloc_pages(size_t cnt, list_head_t* pages)
{
    ASSERT(pmm::GetPhysicalAllocator().GetType() == PMM_TYPE_LIST, "Are you using the wrong allocator?");
    return pmm::GetPhysicalAllocator().Allocate(cnt, (uintptr_t) pages);
}

size_t pmm_free(list_head_t* pages)
{
    ASSERT(pmm::GetPhysicalAllocator().GetType() == PMM_TYPE_LIST, "Are you using the wrong allocator?");
    return pmm::GetPhysicalAllocator().Free((uintptr_t) pages);
}

phys_t pmm_get_phys(page_t* page)
{
    ASSERT(pmm::GetPhysicalAllocator().GetType() == PMM_TYPE_LIST, "Are you using the wrong allocator?");
    return pmm::GetPhysicalAllocator().PageToPhysical((uintptr_t) page);
}
