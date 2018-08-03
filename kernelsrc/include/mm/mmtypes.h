/**
 * @file   mmtypes.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Sunday, April 1st 2018, 3:17:20 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-07-09T13:53:27-04:00
 */

#pragma once

#include "common.h"
#include "list.h"
#include "bitmap.h"
#include "arch/atomic.h"
#include "arch/arch_types.h"

typedef struct mapping mapping_t;
typedef struct region region_t;

struct mapping
{
    struct list list;

    virt_t start;
    size_t size;
    virt_t offset;

};

#define VMR_FLAG_READ  (1 << 0)
#define VMR_FLAG_WRITE (1 << 1)
#define VMR_FLAG_EXEC  (1 << 2)
#define VMR_FLAG_LOCK  (1 << 3)

struct region
{
    struct list list;

    unsigned long size;
    unsigned long flags;
    unsigned long swap_id;
    atomic_t ref;

    uintptr_t* pages;
    region_t* parent;
};