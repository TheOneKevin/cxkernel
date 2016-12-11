/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "memory/pmm.h"
#include "memory/kheap.h"

#include "display/tdisplay.h"
#include "system/PANIC.h"
#include "system/kprintf.h"

uint64_t _length;
uint64_t _addr;
uint32_t* startheap;
extern uint32_t end;
KHEAPBM *kheap;

static uint32_t* framemap  = 0;
static uint32_t nframes    = 0;
uint32_t framestart = 0;

uint32_t pageAlign(const uint32_t addr)
{
    uint32_t a = addr;
    if(addr & 0xFFFFF000)
    {
        a &= 0xFFFFF000;
    }
    return a;
}

void initPmm()
{
    framestart = pageAlign((uint32_t)startheap + (uint32_t)kheap -> fblock -> size + 0x2000);
    
    nframes = (uint32_t) (_addr + _length - framestart) / 0x1000;
    framemap = (uint32_t *) kmalloc(kheap, nframes / 8); //Every 8 bytes (32 bits) we have one entry
    memset(framemap, 0, sizeof(framemap));
    bprintinfo();
    kprintf("Loc: %X ", framemap);
    kprintf("Frame start: %X NFrames: %u\n", framestart, nframes);
}

uint32_t allocFrame()
{
    uint32_t i = 0;
    //First, we check which entry is free. We check this 32 bits at a time
    while(framemap[i] == 0xFFFFFFFF)
    {
        i++;
        if(i == nframes)
        {
            PANIC("No free frames available");
            return -1;
        }
    }
    //Then, we loop through each bit in the entry until we find one free
    for(int n = 0; n < 32;)
    {
        if(((framemap[i] >> n) & 1) == 0)
        {
            framemap[i] |= 1 << n; //Then set it to 1
            return (i * 8 + n) * 0x1000 + framestart; //This will return a PAGE ALIGNED address!
        }
        n++;
    }
    
    PANIC("Something happened");
    return -1; //If nothing works, return error
}

void freeFrame(uint32_t a)
{
    a -= framestart; //Get the offset from the first frame
    if(a == 0) //In case it is the first frame we are freeing
    {
        uint32_t index = (uint32_t)a;
        framemap[index] = 0x0;
    }
    else
    {
        a = a; //Divide by 4KB to get the index to declare free
        uint32_t index = ((uint32_t)a)/0x1000;
        framemap[index] = 0x0;
    }
}