/*
 * File:   arch_types.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 2017-12-06T10:23:34-05:00
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2018-03-28T14:33:03-04:00
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