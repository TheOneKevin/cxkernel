/*
 * Copyright (c) 2019 The cxkernel Authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 * 
 * @file   nodealloc.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on June 05 2019, 11:31 PM
 */

#pragma once

#include "common.h"
#include "core/vm.h"

#ifdef __cplusplus

namespace pmm
{
    class PmmNode : public PhysicalAllocator
    {
    private:
        list_node_t arena_list;
    public:
        PmmNode()
        {
            INIT_LLIST(&arena_list);
        }

        ~PmmNode() { }

        phys_t PageToPhysical(uintptr_t page) override;
        void AddArena(pmm_arena_t* arena, bitmap_t* bt = NULL) override;
        size_t Allocate(size_t cnt, uintptr_t pages) override;
        size_t AllocateSingle(uintptr_t pages) override;
        size_t AllocateContiguous(size_t cnt, uintptr_t pages) override;
        size_t Free(uintptr_t pages) override;
        int GetType() override;
    };

    PhysicalAllocator* GetNodeAllocator();
}

#endif
