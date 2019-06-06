/*
 * Copyright (c) 2019 The cxkernel Authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 * 
 * @file   slab.cc
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on June 04 2019, 10:02 PM
 */

#include <linked_list.h>

typedef struct kmem_slab slab_t;
typedef struct kmem_slabcache slabcache_t;

typedef unsigned int kmem_objctl_t;

struct kmem_slab
{
    list_node_t node;
    void* objects;
    kmem_objctl_t* free;
    unsigned int colour_offset;
    slabcache_t* parent;
};

struct kmem_slabcache
{
    list_node_t list_free;
    list_node_t list_partial;
    list_node_t list_full;

    unsigned int order;         //!< Order of cache
    unsigned int slab_size;     //!< Size of one object
    unsigned int slab_objects;  //!< Number of objects per slab

    unsigned int colour_next;   //!< Next assigned colour (for next slab)
    unsigned int colour_num;    //!< Number of colours. Note that # colours = size of CPU cache / 2^order / n-way associativity.

    list_node_t* node;
};

slabcache_t root_cache;

slabcache_t* kmem_alloc_cache_struct()
{
    slab_t* slab;
    // Do we need more slabs?
    if(list_count(&root_cache.list_free) == 0 && list_count(&root_cache.list_partial) == 0)
    {
        
    }

    // Do we have partial slabs?
    if(list_count(&root_cache.list_partial))
    {
        foreach_llist_entry(slab, node, root_cache.list_partial.next)
        {

        }
    }
    // Do we have any free slabs?
    else if(list_count(&root_cache.list_free))
    {

    }
}

slabcache_t* kmem_new_cache(size_t* size)
{
    slabcache_t* cache = kmem_alloc_cache_struct();
    return cache;
}
