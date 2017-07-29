/*
 * Deals with actual memory :O
 */

#include "memory/pmm.h"
#include "memory/kheap.h"

#include "display/tdisplay.h"
#include "system/PANIC.h"
#include "system/kprintf.h"

struct _BITMAPLLIST
{
    struct _BITMAPLLIST* prev;
    uint32_t* bitmap_paging;
    struct _BITMAPLLIST* next;
}; typedef struct _BITMAPLLIST BITMAPLLIST_t;

struct _PMMBLOCKM
{
    PMMBLOCK_t* paging;
    PMMBLOCK_t* general;
    PMMBLOCK_t* isadma;

    BITMAPLLIST_t* pagingList;
    BITMAPLLIST_t* generaList;
    BITMAPLLIST_t* isadmaList;
}; typedef struct _PMMBLOCKM PMMBLOCKM_t;

PMMBLOCKM_t* pmm;
KHEAPBM* kheap;

//General purpose code below ----------------------------

uint32_t pageAlign(const uint32_t addr)
{
    uint32_t a = addr;
    if(addr & 0xFFFFF000)
    {
        a &= 0xFFFFF000;
    }
    return a;
}

PMMBLOCK_t* descendList(PMMBLOCK_t* node)
{
    PMMBLOCK_t* ptr = node;
    while(ptr -> next != 0)
        ptr = ptr -> next;
    return ptr;
}

BITMAPLLIST_t* descendLlist(BITMAPLLIST_t* node)
{
    BITMAPLLIST_t* ptr = node;
    while(ptr -> next != 0)
        ptr = ptr -> next;
    return ptr;
}

// ===================================================================================== //
//                                    PAGING                                             //
// ===================================================================================== //

int _pmm_allocPaging()
{
    PMMBLOCK_t* ptr = pmm -> paging;
    BITMAPLLIST_t* map = pmm -> pagingList;
    uint32_t i = 0; bool flag = false;
    while(true) //Assume number of bitmaps must equal number of paging blocks!
    {
        //Round frames upwards to nearest 32, then divide by 32 to get number of 32-bit bitmap entries
        for(i = 0; i < (ptr -> frames & 0xFFFFFFE0) / 32 + 1; i++)
        {
            if(map -> bitmap_paging[i] != 0xFFFFFFFF)
            {
                flag = true;
                break;
            }
        }

        if(flag || ptr -> next == 0 || map -> next == 0) break;
        ptr = ptr -> next;
        map = map -> next;
    }

    if(i * 32 + 32 <= ptr -> frames) //If the number of frames (bits in the bitmap) matches the number of 32-bits exactly
    {
        for(int j = 0; j < 32; j++)
        {
            bool bit = (map -> bitmap_paging[i] & (1 << (j))) != 0;
            if(!bit) { map -> bitmap_paging[i] |= 1 << j; return (i * 32 + j) * 0x1000 + ptr -> address; } //32 is size of integer
        }
    }

    else //If the frames end before 1 entry of 32-bits
    {
        for(uint32_t j = 0; j < ptr -> frames - i * 32; j++)
        {
            bool bit = (map -> bitmap_paging[i] & (1 << (j))) != 0;
            if(!bit) { map -> bitmap_paging[i] |= 1 << j; return (i * 32 + j) * 0x1000 + ptr -> address; } //Get the number of frames from i and j
        }
    }

    return -ENOMEM;
}

int _pmm_paging_unalloc(uint32_t addr)
{
    PMMBLOCK_t* ptr = pmm -> paging;
    BITMAPLLIST_t* map = pmm -> pagingList;

    while(ptr -> next != 0 && map -> next != 0)
    {
        if(addr >= ptr -> address && addr <= ptr -> address + 0x1000 * ptr -> frames)
        {
            uint32_t id = ((addr - ptr -> address) / 0x1000 - 1) & 0xFFFFFFE0;
            map -> bitmap_paging[id] |= 1 << ((addr - ptr -> address) / 0x1000 - 1);
            return 0;
        }

        ptr = ptr -> next;
        map = map -> next;
    }

    return -EINVAL;
}

// ===================================================================================== //



//Physical allocations code below ----------------------

void pmm_init()
{
    pmm = (PMMBLOCKM_t*)kmalloc(sizeof(PMMBLOCKM_t));
    pmm -> paging  = 0;
    pmm -> general = 0;
    pmm -> isadma  = 0;

    pmm -> pagingList = 0;
    pmm -> isadmaList = 0;
    pmm -> generaList = 0;
}

void pmm_addBlock(uintptr_t address, size_t nframes, uint8_t type)
{
    PMMBLOCK_t* ptr = (PMMBLOCK_t*)kmalloc(sizeof(PMMBLOCK_t));
    ptr -> prev = 0;
    ptr -> next = 0;
    ptr -> address = address;
    ptr -> frames  = nframes;

    if(type == 1)
    {
        if(pmm -> paging == 0)
        {
            pmm -> paging = ptr;
            pmm -> pagingList = (BITMAPLLIST_t*)kmalloc(sizeof(BITMAPLLIST_t));
            pmm -> pagingList -> bitmap_paging = (uint32_t*)kmalloc(ptr -> frames);
        }

        else
        {
            descendList(pmm -> paging) -> next = ptr;
            ptr -> prev = descendList(pmm -> paging);

            BITMAPLLIST_t* tmp = descendLlist(pmm -> pagingList);
            tmp -> next = (BITMAPLLIST_t*)kmalloc(sizeof(BITMAPLLIST_t));
            tmp -> next -> prev = tmp;
            tmp -> next -> bitmap_paging = (uint32_t*)kmalloc(ptr -> frames);
        }
    }

    else if(type == 2)
    {
        if(pmm -> isadma == 0)
            pmm -> isadma = ptr;

        else
        {
            descendList(pmm -> isadma) -> next = ptr;
            ptr -> prev = descendList(pmm -> isadma);
        }
    }

    else
    {
        if(pmm -> general == 0)
            pmm -> general = ptr;

        else
        {
            descendList(pmm -> general) -> next = ptr;
            ptr -> prev = descendList(pmm -> general);
        }
    }
}

int pmm_allocFrame(uint8_t type)
{
    if(type == 1)
        return _pmm_allocPaging();
    else
        return -EINVAL;
}

int pmm_freeFrame(uint32_t addr)
{
    uint8_t type = 0;
    //Get block type
    PMMBLOCK_t * ptr = pmm -> paging;
    while(ptr -> next != 0)
    {
        if(addr >= ptr -> frames * 0x1000 && addr < (ptr -> frames + ptr -> frames) * 0x1000)
        {
            type = 1;
            break;
        }
        ptr = ptr -> next;
    }

    ptr = pmm -> isadma;
    while(ptr -> next != 0)
    {
        if(addr >= ptr -> frames * 0x1000 && addr < (ptr -> frames + ptr -> frames) * 0x1000)
        {
            type = 2;
            break;
        }
        ptr = ptr -> next;
    }

    ptr = pmm -> general;
    while(ptr -> next != 0)
    {
        if(addr >= ptr -> frames * 0x1000 && addr < (ptr -> frames + ptr -> frames) * 0x1000)
        {
            type = 3;
            break;
        }
        ptr = ptr -> next;
    }

    if(type == 0) return -EINVAL;
    else if(type == 1) return _pmm_paging_unalloc(pageAlign(addr));
    else if(type == 2) return -ENIMPL;
    else if(type == 3) return -ENIMPL;
    else return -EINVAL;
}
