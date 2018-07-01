/*
 * File:   sbrk.c
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 * 
 * Created on Sunday, July 1st 2018, 12:52:07 pm
 * 
 * Last modified by:   Kevin Dai
 * Last modified time: 2018-07-01T14:15:40-04:00
 */

#include "mm/paging.h"
#include "mm/page_alloc.h"
#include "lib/printk.h"
#include "arch/arch_types.h"
#include "arch/arch_paging.h"

static uint32_t amount = 0;
static virt_t brk;

void* sbrk(ptrdiff_t n)
{
    virt_t addr = g_memory_map.KRN_BRK;
    if(n == 0) return (void*) addr;
    g_memory_map.KRN_BRK += n;
    amount += n;
    if(amount > ARCH_PAGE_SIZE) {
        for(uint32_t i = 0; i < amount / ARCH_PAGE_SIZE; i++) {
            phys_t p = pmm_alloc_page();
            map_single_page(brk, p, 0x2);
            brk += ARCH_PAGE_SIZE;
        }
        amount = amount % ARCH_PAGE_SIZE;
    }
    return (void*) addr;
}

void sbrk_init(void)
{
    brk = g_memory_map.KRN_BRK;
}