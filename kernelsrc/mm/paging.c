/*
 * File:   paging.c
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 * 
 * @date Created on Saturday, June 30th 2018, 6:30:06 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-07-02T13:14:16-04:00
 */

#include "mm/paging.h"
#include "arch/arch_paging.h"

int map_single_page(virt_t va, phys_t pa, int flags)
{
    return arch_map_page(va, pa, flags);
}

phys_t get_phys_addr(virt_t va)
{
    return arch_get_phys(va);
}