/*
 * File:   sbrk.c
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 * 
 * @date Created on Sunday, July 1st 2018, 12:52:07 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-07-05T16:45:25-04:00
 */

#include "mm/paging.h"
#include "mm/page_alloc.h"
#include "lib/printk.h"
#include "arch/arch_types.h"
#include "arch/arch_paging.h"

static uint32_t pos = ARCH_PAGE_SIZE;
static uint32_t neg = ARCH_PAGE_SIZE;
static virt_t brk;

void *sbrk(ptrdiff_t n)
{
    virt_t addr = g_memory_map.KRN_BRK;
    g_memory_map.KRN_BRK += n;

    if (n == 0)
        return (void *)addr;
    else if (n > 0)
    {
        pos += n;
        if (pos > ARCH_PAGE_SIZE)
        {
            for (uint32_t i = 0; i < pos / ARCH_PAGE_SIZE; i++)
            {
                phys_t p = pmm_alloc_page();
                map_single_page(brk, p, 0x3);
                brk += ARCH_PAGE_SIZE;
            }
            pos = pos % ARCH_PAGE_SIZE;
        }
    }
    else if (n < 0)
    {
        neg -= n;
        if(neg > ARCH_PAGE_SIZE)
        {
            for(uint32_t i = 0; i < neg / ARCH_PAGE_SIZE; i++)
            {
                brk -= ARCH_PAGE_SIZE;
                pmm_free_page(get_phys_addr(brk));
                map_single_page(brk, 0, 0x0);
            }
            neg = neg % ARCH_PAGE_SIZE;
        }
    }
    return (void *)addr;
}

void sbrk_init(void)
{
    brk = g_memory_map.KRN_BRK;
}