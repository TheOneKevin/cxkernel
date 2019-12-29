/**
 * Copyright (c) 2019 The cxkernel Authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 * 
 * @file   native.cc
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on December 28 2019, 11:18 AM
 */

#include "unity/unity.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "core/vm.h"
#include "core/pmm_node.h"

#include "../core/mm/NodeAllocator.cc"
#include "../core/mm/PhysicalAllocator.cc"

static page_t pages[1024];
static uint32_t bitarray[32];

static pmm_arena_t arena;
static bitmap_t bitmap;

extern "C" void setUp()
{
    // Setup environment
    // std::cout << "SETUP" << std::endl;
    arena = {
        .node = { NULL, NULL },
        .flags = CX_ARENA_PRESENT,
        .base = 0,
        .size = 1024,
        .free = 0,
        .priority = 1,
        .pages = pages,
        .free_list = { NULL, NULL }
    };
    bitmap = {
        .length = 32,
        .bit_count = 1024,
        .bitmap = bitarray
    };

    for(int i = 1; i < 31; i++)
        bitarray[i] = 0xFFFFFFFF;
    auto allocator = new pmm::PmmNode();
    pmm::SetPhysicalAllocator(allocator);
    pmm::GetPhysicalAllocator().AddArena(&arena, &bitmap);
}

extern "C" void tearDown()
{
    // Clean up environment
    // std::cout << "TEARDOWN" << std::endl;
    delete &pmm::GetPhysicalAllocator();
    memset(bitarray, 0, sizeof(bitarray));
    memset(pages, 0, sizeof(pages));
}

extern "C" void Test_AllocationEdge()
{
    const size_t alloc = 2*32;
    void* p = alloca(pmm::GetPhysicalAllocator().GetSize());
    size_t sz = pmm::GetPhysicalAllocator().Allocate(alloc, (uintptr_t) p);
    TEST_ASSERT_EQUAL_size_t(alloc, sz);
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(Test_AllocationEdge);
    RUN_TEST(Test_AllocationEdge);
    return UNITY_END();
}