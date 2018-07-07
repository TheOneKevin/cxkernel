/**
 * @file
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Wednesday, December 6th 2017, 10:23:34 am
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-07-05T22:29:56-04:00
 */

#pragma once

#include "common.h"

typedef uint32_t virt_t;
typedef uint64_t phys_t; // We still want to support PAE address
struct MEMORY_MAP
{
    virt_t KRN_MMAP;
    virt_t KRN_MMAP_END;
    virt_t KRN_PRIV_BEGIN;
    virt_t KRN_PRIV_END;
    virt_t KRN_BEGIN;
    virt_t KRN_END;
    virt_t KRN_BRK;
    virt_t KRN_SLIM;
    uint32_t KRN_STACK_SIZE;
    virt_t KRN_STACK_BEGIN;
    virt_t KRN_PAGE_TABLES_BEGIN;

    phys_t MOD_END;
};

typedef struct PT_REGS
{

} arch_t;