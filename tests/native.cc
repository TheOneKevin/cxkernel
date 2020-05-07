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

#include <random>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#define OS_DBG(f_, ...) printf((f_), ##__VA_ARGS__)
#define OS_LOG(f_, ...) printf((f_), ##__VA_ARGS__)
#define OS_PRN(f_, ...) printf((f_), ##__VA_ARGS__)

#include "core/vm.h"
#include "core/pmm_node.h"

#include "../core/mm/NodeAllocator.cc"
#include "../core/mm/PhysicalAllocator.cc"

static page_t pages[1024];
static uint32_t bitarray[32];

static pmm_arena_t arena;
static bitmap_t bitmap;

// Overkill
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dis(1, 1024);

static void clear_random_n_bits(int nfree)
{
    for(int i = 0; i < nfree; i++)
    {
        int idx = dis(gen);
        while(!bitmap_tstbit(bitarray, idx))
            idx = dis(gen);
        bitmap_clrbit(bitarray, idx);
    }
}

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

    auto allocator = new pmm::PmmNode();
    pmm::SetPhysicalAllocator(allocator);
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
    // Randomly clear nfree bits
    int nfree = dis(gen);
    memset(bitarray, -1, sizeof(bitarray));
    clear_random_n_bits(nfree);
    // Add to allocator
    pmm::GetPhysicalAllocator().add_arena(&arena, &bitmap);
    TEST_ASSERT_EQUAL_size_t(nfree, arena.free);
    // Test!
    void* p = alloca(pmm::GetPhysicalAllocator().get_size());
    size_t sz = pmm::GetPhysicalAllocator().allocate(nfree, (uintptr_t) p);
    TEST_ASSERT_EQUAL_size_t(nfree, sz);
    TEST_ASSERT_EQUAL_size_t(arena.free, (size_t) list_count(arena.free_list.next));
}

extern "C" void Test_AllocFree()
{
    // Randomly clear nfree bits
    int nfree = dis(gen);
    memset(bitarray, -1, sizeof(bitarray));
    clear_random_n_bits(nfree);
    // Add to allocator
    pmm::GetPhysicalAllocator().add_arena(&arena, &bitmap);
    TEST_ASSERT_EQUAL_size_t(nfree, arena.free);
    // Test!
    void* p = alloca(pmm::GetPhysicalAllocator().get_size());
    for(int i = 0; i < 10000; i++)
    {
        memset(p, 0, pmm::GetPhysicalAllocator().get_size());
        size_t sz = pmm::GetPhysicalAllocator().allocate(nfree, (uintptr_t) p);
        TEST_ASSERT_EQUAL_size_t(nfree, sz);
        TEST_ASSERT_EQUAL_size_t(0, arena.free);
        TEST_ASSERT_EQUAL_size_t(arena.free, (size_t) list_count(arena.free_list.next));
        pmm::GetPhysicalAllocator().free((uintptr_t) p);
        TEST_ASSERT_EQUAL_size_t(nfree, arena.free);
        TEST_ASSERT_EQUAL_size_t(arena.free, (size_t) list_count(arena.free_list.next));
    }
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(Test_AllocationEdge);
    RUN_TEST(Test_AllocFree);
    return UNITY_END();
}