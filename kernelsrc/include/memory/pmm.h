/*
 * File:   pmm.h
 * Author: Kevin
 *
 * Created on September 24, 2016, 1:52 PM
 */

#include "system.h"

#ifndef PMM_H
#define PMM_H

#ifdef __cplusplus
extern "C" {
#endif

// Initializes the physical memory manager
void pmm_init();

/// Allocates a frame for use.
/// \param type The type of frame to be allocated (1 = paging, 2 = ISA/DMA, 3 = General purpose, all other numbers will be regarded as 3)
/// \return Address of the allocated frame
int pmm_allocFrame(uint8_t);

// Frees a frame.
int pmm_freeFrame(uint32_t);

/// Adds a block of memory for the physical memory manager to use.
/// \param addr The virtual address of the block
/// \param size The size of the block
/// \param type The type of memory (1 = paging, 2 = ISA/DMA, 3 = General purpose, all other numbers will be regarded as 3)
void pmm_addBlock(uintptr_t, size_t, uint8_t);

// Prints the physical memory manager information
//void _pmm_printInfo();

// Page aligns an address
uint32_t pageAlign(uint32_t addr);


struct PMMBLOCKSTRUCT
{
    struct PMMBLOCKSTRUCT* prev;
    uint32_t    frames;
    uint32_t    address;
    struct PMMBLOCKSTRUCT* next;
}; typedef struct PMMBLOCKSTRUCT PMMBLOCK_t;

#ifdef __cplusplus
}
#endif

#endif /* PMM_H */
