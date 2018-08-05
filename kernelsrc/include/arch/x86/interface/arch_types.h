/**
 * @file
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Wednesday, December 6th 2017, 10:23:34 am
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-08-04T22:17:26-04:00
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

typedef struct
{
    uint32_t gs, fs, es, ds;                             // Pushed the segs last
    uint32_t edi, esi, ebp, useless, ebx, edx, ecx, eax; // Pushed by 'pusha'
    uint32_t int_no, err_code;                           // Our 'push byte #' and ecodes do this
    uint32_t eip, cs, eflags, esp, ss;                   // Pushed by the processor automatically
} regs_t;

struct PT_REGS
{
    void* registers;
};
