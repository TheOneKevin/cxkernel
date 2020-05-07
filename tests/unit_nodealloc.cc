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
#include <chrono>

#define OS_DBG(f_, ...) printf((f_), ##__VA_ARGS__)
#define OS_LOG(f_, ...) printf((f_), ##__VA_ARGS__)
#define OS_PRN(f_, ...) printf((f_), ##__VA_ARGS__)
#define ASSERT

#include "core/vm.h"
#include "core/pmm_node.h"

#include "../core/mm/NodeAllocator.cc"
#include "../core/mm/PhysicalAllocator.cc"
#include "../core/mm/pmm.cc"

#define MAX_ITER 140000

static page_t pages[1024];
static uint32_t bitarray[32];

static pmm_arena_t arena;
static bitmap_t bitmap;

// Overkill
std::random_device rd;
std::mt19937 gen(rd());

static inline int random_range(int min, int max)
{
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

static void clear_random_n_bits(int nfree)
{
    for(int i = 0; i < nfree; i++)
    {
        int idx = random_range(0, 1023);
        while(!bitmap_tstbit(bitarray, idx))
            idx = random_range(0, 1023);
        bitmap_clrbit(bitarray, idx);
    }
}

static void clear_n_bits_contig(int nfree)
{
    int st = random_range(0, 1023-nfree);
    for(int i = 0; i < nfree; i++)
        bitmap_clrbit(bitarray, i+st);
}

static void prealloc_checks(size_t nfree)
{
    TEST_ASSERT_EQUAL_size_t(nfree, arena.free);
    TEST_ASSERT_EQUAL_size_t(arena.free, (size_t) list_count(&arena.free_list));
}

static void postalloc_checks(size_t nfree, size_t alloc, size_t sz, list_head_t* head)
{
    TEST_ASSERT_EQUAL_size_t(nfree-sz, arena.free);
    TEST_ASSERT_EQUAL_size_t(arena.free, (size_t) list_count(&arena.free_list));
    TEST_ASSERT_EQUAL_size_t(sz, list_count(head));
}

static void postfree_checks(size_t sz_allocd, size_t sz, list_head_t* head)
{
    TEST_ASSERT_EQUAL_size_t(sz_allocd, sz);
    TEST_ASSERT_EQUAL_size_t(sz_allocd, arena.free);
    TEST_ASSERT_EQUAL_size_t(arena.free, (size_t) list_count(&arena.free_list));
    TEST_ASSERT_EQUAL_size_t(0, list_count(head));
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
    pmm::set_allocator(allocator);
}

extern "C" void tearDown()
{
    // Clean up environment
    // std::cout << "TEARDOWN" << std::endl;
    delete &pmm::get_allocator();
    memset(bitarray, 0, sizeof(bitarray));
    memset(pages, 0, sizeof(pages));
}

extern "C" void TestAllocationEdge()
{
    // Randomly clear nfree bits
    int nfree = random_range(384, 680);
    memset(bitarray, -1, sizeof(bitarray));
    clear_random_n_bits(nfree);

    list_head_t* p = NULL;
    list_head_t head; INIT_LLIST(&head);

    pmm::get_allocator().add_arena(&arena, &bitmap);
    prealloc_checks(nfree);
    size_t sz = pmm::get_allocator().allocate(nfree, (uintptr_t) &p);
    head.next = p;
    TEST_ASSERT_EQUAL_size_t(nfree, sz);
    postalloc_checks(nfree, nfree, sz, &head);
}

extern "C" void TestAllocCont()
{
    // Randomly clear some bits
    memset(bitarray, -1, sizeof(bitarray));
    //clear_random_n_bits(random_range(384, 680));
    // Ensure there is a run of contiguous memory
    int nfree = random_range(384, 680);
    clear_n_bits_contig(nfree);

    list_head_t* p = NULL;
    list_head_t head; INIT_LLIST(&head);

    pmm::get_allocator().add_arena(&arena, &bitmap);
    prealloc_checks(nfree);
    size_t sz = pmm::get_allocator().allocate_contiguous(nfree, (uintptr_t) &p);
    head.next = p;
    TEST_ASSERT_EQUAL_size_t(nfree, sz);
    postalloc_checks(nfree, nfree, sz, &head);
}

extern "C" void TestAllocFree()
{
    using namespace std::chrono;
    int64_t avg1 = 0, avg2 = 0;

    // Randomly clear nfree bits
    int nfree = random_range(384, 680);
    memset(bitarray, -1, sizeof(bitarray));
    clear_random_n_bits(nfree);

    list_head_t* p = NULL;
    list_head_t head; INIT_LLIST(&head);
    pmm::get_allocator().add_arena(&arena, &bitmap);
    prealloc_checks(nfree);
    
    for(int i = 0; i < MAX_ITER; i++)
    {
        auto t1 = high_resolution_clock::now();
        size_t sz1 = pmm::get_allocator().allocate(nfree, (uintptr_t) &p);
        auto t2 = high_resolution_clock::now();
        avg1 += duration_cast<std::chrono::nanoseconds>(t2-t1).count();

        head.next = p;
        TEST_ASSERT_EQUAL_size_t(nfree, sz1);
        postalloc_checks(nfree, nfree, sz1, &head);

        t1 = high_resolution_clock::now();
        size_t sz2 = pmm::get_allocator().free((uintptr_t) &p);
        t2 = high_resolution_clock::now();
        avg2 += duration_cast<std::chrono::nanoseconds>(t2-t1).count();
        
        head.next = p;
        postfree_checks(sz1, sz2, &head);
    }

    std::cout << "Average time taken to allocate: " << (double) avg1 / 140000.0 << " ns" << std::endl;
    std::cout << "Average time taken to free: " << (double) avg2 / 140000.0 << " ns" << std::endl;
}

extern "C" void TestAllocError()
{
    // Randomly clear nfree bits
    int nfree = random_range(384, 680);
    int alloc_size = random_range(1, 100);
    TEST_ASSERT_NOT_EQUAL(alloc_size + nfree, nfree);
    memset(bitarray, -1, sizeof(bitarray));
    clear_random_n_bits(nfree);
    
    list_head_t* p = NULL;
    list_head_t head; INIT_LLIST(&head);
    
    pmm::get_allocator().add_arena(&arena, &bitmap);
    prealloc_checks(nfree);
    size_t sz1 = pmm::get_allocator().allocate(nfree + alloc_size, (uintptr_t) &p);
    head.next = p;
    TEST_ASSERT_EQUAL_size_t(nfree, sz1);
    postalloc_checks(nfree, nfree, sz1, &head);
    TEST_ASSERT_NOT_EQUAL(nfree + alloc_size, sz1);

    size_t sz2 = pmm::get_allocator().free((uintptr_t) &p);
    head.next = p;
    postfree_checks(sz1, sz2, &head);
}

extern "C" void TestDoublefree()
{

}

int main(int argc, char* argv[])
{
    UNITY_BEGIN();
    if(argc == 1)
        return UNITY_END();
    switch(atoi(argv[1]))
    {
        case 0:
            RUN_TEST(TestAllocationEdge);
            break;
        case 1:
            RUN_TEST(TestAllocFree);
            break;
        case 2:
            for(int i = 0; i < MAX_ITER; i++)
                RUN_TEST(TestAllocError);
            break;
        case 3:
            RUN_TEST(TestDoubleFree);
            break;
        case 4:
            for(int i = 0; i < MAX_ITER; i++)
                RUN_TEST(TestAllocCont);
            break;
    }
    return UNITY_END();
}