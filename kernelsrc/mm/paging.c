/*
 * File:   paging.c
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 * 
 * Created on Saturday, June 30th 2018, 6:30:06 pm
 * 
 * Last modified by:   Kevin Dai
 * Last modified time: 2018-06-30T18:41:21-04:00
 */

#include "mm/paging.h"
#include "arch/arch_paging.h"

int map_single_page(virt_t va, phys_t pa, int flags)
{
    if(arch_map_page(va, pa, flags) < 0) return -1;
    return 0;
}