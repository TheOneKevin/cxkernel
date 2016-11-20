/*
 *   2014 Leonard Kevin McGuire Jr (www.kmcg3413.net) (kmcg3413@gmail.com)
 */

#include "memory/kheap.h"
#include "system/kprintf.h"
 
void k_heapBMInit(KHEAPBM *heap)
{
    heap->fblock = 0;
}
 
int k_heapBMAddBlock(KHEAPBM *heap, uintptr_t addr, size_t size, size_t bsize)
{
    KHEAPBLOCKBM    *b;
    size_t          bcnt;
    size_t          x;
    uint8_t         *bm;

    b = (KHEAPBLOCKBM*)addr;
    b->size = size - sizeof(KHEAPBLOCKBM);
    b->bsize = bsize;

    b->next = heap->fblock;
    heap->fblock = b;

    bcnt = size / bsize;
    bm = (uint8_t*)&b[1];

    // Clear bitmap
    for (x = 0; x < bcnt; ++x)
        bm[x] = 0;
 
    // Reserve room for bitmap
    bcnt = (bcnt / bsize) * bsize < bcnt ? bcnt / bsize + 1 : bcnt / bsize;
    for (x = 0; x < bcnt; ++x)
        bm[x] = 5;
    
    b->lfb = bcnt - 1;
    b->used = bcnt;
    
    return 1;
}
 
static uint8_t k_heapBMGetNID(uint8_t a, uint8_t b)
{
    uint8_t c;
    for (c = a + 1; c == b || c == 0; ++c);
    return c;
}
 
void *k_heapBMAlloc(KHEAPBM *heap, size_t size)
{
    KHEAPBLOCKBM    *b;
    uint8_t         *bm;
    size_t          bcnt;
    size_t          x, y, z;
    size_t          bneed;
    uint8_t         nid;

    // Iterate blocks
    for (b = heap->fblock; b; b = b->next)
    {
        // Check if block has enough room
        if (b->size - (b->used * b->bsize) >= size)
        {
            bcnt = b->size / b->bsize;		
            bneed = (size / b->bsize) * b->bsize < size ? size / b->bsize + 1 : size / b->bsize;
            bm = (uint8_t*)&b[1];
            
            for (x = (b->lfb + 1 >= bcnt ? 0 : b->lfb + 1); x != b->lfb; ++x)
            {
                // Just wrap around
                if (x >= bcnt)
                    x = 0;
                if (bm[x] == 0)
                {
                    for (y = 0; bm[x + y] == 0 && y < bneed && (x + y) < bcnt; ++y); // Count free blocks
                    // We have enough, now allocate them
                    if (y == bneed)
                    {
                        // Find ID that does not match left or right
                        nid = k_heapBMGetNID(bm[x - 1], bm[x + y]);
                        // Allocate by setting id
                        for (z = 0; z < y; ++z)
                            bm[x + z] = nid;
                        // Optimization
                        b->lfb = (x + bneed) - 2;
                        // Count used blocks NOT bytes
                        b->used += y;
                        return (void*)(x * b->bsize + (uintptr_t)&b[1]);
                    }
                    // x will be incremented by one ONCE more in our FOR loop
                    x += (y - 1);
                    continue;
                } //End if
            } //End for
        } //End if
    } //End for
    
    return 0;
}
 
int k_heapBMFree(KHEAPBM *heap, void *ptr)
{
    KHEAPBLOCKBM    *b;	
    uintptr_t       ptroff;
    uint32_t        bi, x;
    uint8_t         *bm;
    uint8_t         id;
    uint32_t        max;

    for (b = heap->fblock; b; b = b->next)
    {
        if ((uintptr_t)ptr > (uintptr_t)b && (uintptr_t)ptr < (uintptr_t)b + b->size)
        {
            // Found block
            ptroff = (uintptr_t)ptr - (uintptr_t)&b[1];  // Get offset to get block
            // Block offset in BM
            bi = ptroff / b->bsize;
            /* .. */
            bm = (uint8_t*)&b[1];
            // Clear allocation
            id = bm[bi];
            // Oddly GCC did not optimize this...
            max = b->size / b->bsize;
            for (x = bi; bm[x] == id && x < max; ++x)
                bm[x] = 0;
            
            // Update free block count
            b->used -= x - bi;
            return 1;
        }
    }
    
    return 0;
}

void *kmalloc(KHEAPBM *heap, size_t size)
{
    return k_heapBMAlloc(heap, size);
}

void kfree(KHEAPBM *heap, void *ptr)
{
    int ret = k_heapBMFree(heap, ptr);
    if(ret == 0)
        kprintf("Cannot free block!"); // TODO: This error needs to be raised or reported somehow
}