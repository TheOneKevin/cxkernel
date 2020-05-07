/*
 * Copyright (c) 2019 The cxkernel Authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 * 
 * @file   bootalloc.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on June 05 2019, 9:39 PM
 */

#pragma once

#include "common.h"
#include <bitmap.h>
#include "arch/types.h"
#include "core/vm.h"

#ifdef __cplusplus

namespace pmm
{
    typedef struct
    {
        phys_t addr;
        int pages;
    } bafree_t;

    class BootAllocator : public PhysicalAllocator
    {
    private:
        bitmap_t* alloc_map;
        phys_t _ptr;
        bool update_all(void);

    public:
        // Other stuff
        void add_arena(pmm_arena_t*, bitmap_t* bt = NULL) override;
        phys_t page_to_physical(uintptr_t page) override;
        size_t allocate_single(uintptr_t page) override;
        size_t free(uintptr_t st) override;
        int get_type() override;
        size_t get_size() override;
    };

    PhysicalAllocator* get_bootallocator();
}

#endif
