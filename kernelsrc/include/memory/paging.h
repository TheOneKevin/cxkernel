/*
 * File:   paging.h
 * Author: Kevin
 *
 * Created on September 20, 2016, 6:04 AM
 */

#include "system.h"

#ifndef PAGING_H
#define PAGING_H

#ifdef __cplusplus
extern "C" {
#endif

#define PAGE_MASK         0xFFFFF000
#define PAGE_SIZE         0x1000

void paging_init();
status_t paging_map_virtual_to_phys(const uint32_t virt, const uint32_t phys, const uint32_t flags);
void paging_unalloc(uint32_t virt);
uint32_t get_physaddr(uint32_t virt);

#ifdef __cplusplus
}
#endif

#endif /* PAGING_H */
