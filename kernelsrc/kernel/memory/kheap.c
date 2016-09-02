/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "memory/kheap.h"

extern uint32_t end;
uint32_t addrPtr = (uint32_t)&end;

uint32_t h_kmalloc(size_t size, bool align, uint32_t *physicalAddr)
{
    if(align && (addrPtr & PAGE_ALIGN))
    {
        //Align
        addrPtr &= PAGE_ALIGN;
        addrPtr += PAGE_SIZE;
    }
    if(physicalAddr) //If physicalAddr is not 0
    {
        *physicalAddr = addrPtr;
    }
    uint32_t ret = addrPtr;
    addrPtr += size;
    return ret;
}