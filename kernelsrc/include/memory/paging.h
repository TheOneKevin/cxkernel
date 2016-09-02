/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   paging.h
 * Author: Kevin
 *
 * Created on August 27, 2016, 1:48 PM
 */

#include "common.h"
#include "arch/exceptions.h"
#include "system/irq.h"
#include "memory/kheap.h"


#ifndef PAGING_H
#define PAGING_H

#ifdef __cplusplus
extern "C" {
#endif

// Macros used in the bitset algorithms.
#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))
    
//The paging tutorial up on the OSDev Wiki used a fixed space
//for the paging tables and directory, but we don't want anything fixed
//so we use structures and pointers :)

typedef struct page
{
    uint32_t present  : 1;
    uint32_t rw       : 1;
    uint32_t user     : 1;
    uint32_t accessed : 1;
    uint32_t dirty    : 1;
    uint32_t unused   : 7;
    uint32_t frame    : 20;
} page_t;

typedef struct page_table
{
    page_t pages[1024];
} page_table_t;

typedef struct page_directory
{
    page_table_t *tables[1024]; //Array of pointers to the page tables
    uint32_t tablesPhys[1024];  //Pointers to the (physical address) page tables
    uint32_t physAddr;          //Physical address of the array above
} page_directory_t;

void setup_paging();

void load_page_dir(page_directory_t *dir);
void enable_paging();
/**
 * Gets a page from the address and page directory
 * @param addr Page address
 * @param createPage If the page isn't found, create one anyways?
 * @param dir Directory of the page
 * @return Page info inside structure
 */
page_t *getPage(uint32_t addr, bool createPage, page_directory_t *dir);

void page_fault(regs_t *regs);

#ifdef __cplusplus
}
#endif

#endif /* PAGING_H */

