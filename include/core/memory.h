/**
 * Copyright (c) 2019 The cxkernel Authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 * 
 * @file   memory.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on July 16 2019, 10:35 AM
 */

#pragma once

#include "common.h"
#include "core/vm.h"

__BEGIN_CDECLS

typedef struct kmem_slab        slab_t;
typedef struct kmem_slabcache   slabcache_t;
typedef struct kmem_aspace      kmem_aspace_t;
typedef struct kmem_free_block  kmem_free_block_t;

struct kmem_slab
{
    list_node_t node;
    void* objects;
    kmem_objctl_t free;
    unsigned int colour_offset;
    unsigned int inuse;
    slabcache_t* parent;
};

struct kmem_slabcache
{
    list_head_t list_free;
    list_head_t list_partial;
    list_head_t list_full;

    unsigned int order;         //!< Order of cache
    unsigned int slab_size;     //!< Size of one object
    unsigned int slab_objects;  //!< Number of objects per slab

    unsigned int colour_next;   //!< Next assigned colour (for next slab)
    unsigned int colour_num;    //!< Number of colours. Note that # colours = size of CPU cache / 2^order / n-way associativity.

    list_node_t node;           //!< Links all caches together
};

//! A structure to contain all free kernel address blocks 
struct kmem_aspace
{
    virt_t base;
    size_t size;
    list_head_t free_list;
};

//! A structure to describe a free block of kernel addresses
struct kmem_free_block
{
    list_node_t node;   //!< Node for free list
    virt_t base;        //!< Base virtual address of block
    size_t size;        //!< Size of virtual address block
};

void* vmalloc(size_t size);
void vfree(void*);

__END_CDECLS

namespace kmem
{
    void init();
    slabcache_t* new_cache(size_t);
    void* cache_alloc(size_t);
    void cache_free(slabcache_t*, void*);
}
