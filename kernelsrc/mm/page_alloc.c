/*
 * File:   page_alloc.c
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 2017-08-30T14:37:09-04:00
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2018-04-01T20:32:07-04:00
*/

#include "panic.h"
#include "mm/page_alloc.h"
#include "lib/printk.h"

#include "arch/arch_types.h"
#include "arch/arch_paging.h"

static phys_t _ptr;

// If we run out of room, we search the bitmap again to find
// a single free page frame.
bool pmm_update_all(void)
{
    for(size_t i = g_pmm_buddy_map -> length; i > 0; i--)
    {
        if(g_pmm_buddy_map -> bitmap[i - 1] != 0xFFFFFFFF)
        {
            for(_ptr = 32; _ptr > 0; _ptr--)
                if(!bitmap_tstbit(g_pmm_buddy_map -> bitmap, _ptr - 1 + (i - 1) * sizeof(unsigned int) * 8))
                    break;
            _ptr += (i - 1) * sizeof(unsigned int) * 8;
            return true;
        }
    }

    PANIC("No memory left on init!\n");
    return false;
}

phys_t pmm_alloc_page(void)
{
    phys_t foo = 0;
    if(bitmap_tstbit(g_pmm_buddy_map -> bitmap, _ptr)) // If it is not a free page, find one
        pmm_update_all();
    foo = _ptr;
    if(_ptr > 0) _ptr--;
    bitmap_setbit(g_pmm_buddy_map -> bitmap, foo);
    //OS_LOG("Allocated 1 page at 0x%X\n", foo * ARCH_PAGE_SIZE);
    return foo * ARCH_PAGE_SIZE;
}

void pmm_free_page(phys_t address)
{
    bitmap_clrbit(g_pmm_buddy_map -> bitmap, ARCH_PAGE_ALIGN(address) / ARCH_PAGE_SIZE - 1);
}

void pmm_free_page_multi(phys_t address, int pages)
{
    for(uint64_t a = address; a <= address + (uint64_t)pages * ARCH_PAGE_SIZE; a++)
        bitmap_clrbit(g_pmm_buddy_map -> bitmap, ARCH_PAGE_ALIGN(a) / ARCH_PAGE_SIZE - 1);
}

void pmm_init(void)
{

}