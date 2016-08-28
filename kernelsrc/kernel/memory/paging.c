/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "memory/paging.h"

uint32_t *frames;
uint32_t nframes;
uint32_t pageSize;
page_directory_t *current_dir;
extern uint32_t placement_address;

void enable_paging()
{
    uint32_t cr0;
    asm volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;
    asm volatile("mov %0, %%cr0" :: "r"(cr0));
}

void load_page_dir(page_directory_t *dir)
{
    current_dir = dir;
    asm volatile("mov %0, %%cr3" :: "r"(&dir -> tablesPhys));
}

void setup_paging()
{
    uint64_t end_of_page = length + addr;
    nframes = (uint32_t) end_of_page / PAGE_SIZE;
}