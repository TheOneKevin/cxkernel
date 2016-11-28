/* 
 * File:   kheap.h
 * Author: Kevin Dai, based off of kheap.c
 *
 * Created on August 29, 2016, 11:36 AM
 */

#include "common.h"

#ifndef KHEAP_H
#define KHEAP_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _KHEAPBLOCKBM
{
    struct _KHEAPBLOCKBM       *next;
    size_t			size;
    uint32_t			used;
    size_t			bsize;
    uint32_t                    lfb;
} KHEAPBLOCKBM;
 
typedef struct _KHEAPBM {
    KHEAPBLOCKBM    *fblock;
} KHEAPBM;

void k_heapBMInit(KHEAPBM *heap);
int k_heapBMAddBlock(KHEAPBM *heap, uintptr_t addr, uint32_t size, uint32_t bsize);
//void *k_heapBMAlloc(KHEAPBM *heap, uint32_t size);
//void k_heapBMFree(KHEAPBM *heap, void *ptr);
void *kmalloc(KHEAPBM *heap, size_t size);
void kfree(KHEAPBM *heap, void *ptr);

extern KHEAPBM* kheap; //Kernel heap

#ifdef __cplusplus
}
#endif

#endif /* KHEAP_H */

