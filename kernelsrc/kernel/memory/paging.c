/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "memory/paging.h"

// Our frames bitset/bitmap
uint32_t *frames;
uint32_t nframes;

extern uint32_t addrPtr;

void enable_paging()
{
    // We enable paging by or-ing our CR0 register
    uint32_t cr0;
    asm volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;
    asm volatile("mov %0, %%cr0" :: "r"(cr0));
}

void load_page_dir(page_directory_t *dir)
{
    // We load the directory into the CR3 register
    current_dir = dir;
    asm volatile("mov %0, %%cr3" :: "r"(&dir -> tablesPhys));
}

void invlpg(uint32_t virt_addr)
{
    asm volatile("invlpg (%0)" :: "a" (virt_addr));
}

// Set frame inside our frames bitmap
static void set_frame(uint32_t frame_addr)
{
    // We first get the frame number from the address
    uint32_t frame  = frame_addr / PAGE_SIZE;
    // We get the index to our bitmap from our page index
    uint32_t index  = INDEX_FROM_BIT(frame);
    // We get the offset of our bitmap from our page index
    uint32_t offset = OFFSET_FROM_BIT(frame);
    // We mark the frame as used
    frames[index] |= (0x1 << offset);
}
// Clear frame from our frames bitmap
static void clear_frame(uint32_t frame_addr)
{
    uint32_t frame  = frame_addr / PAGE_SIZE;
    uint32_t index  = INDEX_FROM_BIT(frame);
    uint32_t offset = OFFSET_FROM_BIT(frame);
    frames[index] &= ~(0x1 << offset);
}
// Test frame from our frames bitmap
static uint32_t test_frame(uint32_t frame_addr)
{
    uint32_t frame  = frame_addr / PAGE_SIZE;
    uint32_t index  = INDEX_FROM_BIT(frame);
    uint32_t offset = OFFSET_FROM_BIT(frame);
    return (frames[index] & (0x1 << offset));
}

static uint32_t first_free_frame()
{
    uint32_t i, j;
    // Loop through all the frames in our bitmap
    for(i = 0; i < INDEX_FROM_BIT(nframes); i++)
    {
        // If our current frame index bitmap thing doesn't have
        // any free entries, exit
        if(frames[i] != 0xFFFFFFFF)
        {
            // Else, we find the one that's free
            for(j = 0; j < 32; j++)
            {
                uint32_t toTest = 0x1 << j;
                if(!(frames[i] & toTest))
                    return i * 4 * 8 + j;
            }
        }
    }
    return -1;
}

void alloc_frame(page_t *page, bool isK, bool rw)
{
    // Get the frame from our page, and see if it's taken
    if(page -> frame != 0)
        return;
    else
    {
        uint32_t index = first_free_frame();
        if(index == (uint32_t)-1)
        {
            bprinterr(); console_writeline("No free frames available!");
            PANIC("No free frames available!");
        }
        set_frame(index * PAGE_SIZE);
        page -> present = 1;
        page -> rw = (rw) ? 1 : 0;
        page -> user = (isK) ? 0 : 1;
        page -> frame = index;
    }
}

void free_frame(page_t *page)
{
    uint32_t frame;
    // Store the page's frame into our frame variable
    if(!(frame = page -> frame))
        return; //If there's no allocated frame, then return
    else
    {
        //Else, we free our frame
        clear_frame(frame);
        page->frame = 0x0; //And set the frame flag to 0
    }
}

page_t *getPage(uint32_t addr, bool createPage, page_directory_t *dir)
{
    addr /= PAGE_SIZE;
    uint32_t index = addr / 1024;
    if(dir->tables[index])
    {
        //Return pointer to the page within the directory table
        //Then get page from a table of pages (every 1024)
        return &dir -> tables[index] -> pages[addr % 1024];
    }
    else if(createPage)
    {
        uint32_t tmp;
        dir -> tables[index] = (page_table_t*)h_kmalloc(sizeof(page_table_t), 1, &tmp);
        dir -> tablesPhys[index] = tmp | 0x7; //Set present bit, r/w, user
        return &dir -> tables[index] -> pages[addr % 1024];
    }
    else
        return 0; //Return 0 if no page was found and the user didn't want one made
}

void page_fault(regs_t *r)
{
    uint32_t fault_addr;
    asm volatile("mov %%cr2, %0" : "=r" (fault_addr));
    
   // The error code gives us details of what happened.
   int errcode = r -> err_code;
   int p        = (1 << 0) & errcode;
   int rw       = (1 << 1) & errcode;
   int us       = (1 << 2) & errcode;
   console_putc('\n');
   regdump(r); //Dump regs
   // Output an error message.
   console_write("\n");
   console_write("Page fault!\n");
   
   if(!us && !rw && !p) console_write("Supervisory process tried to read a non-present page entry!");
   if(!us && !rw &&  p) console_write("Supervisory process tried to read a page and caused a protection fault!");
   if(!us &&  rw && !p) console_write("Supervisory process tried to write to a non-present page entry!");
   if(!us &&  rw &&  p) console_write("Supervisory process tried to write a page and caused a protection fault!");
   if(us  && !rw && !p) console_write("User process tried to read a non-present page entry!");
   if(us  && !rw &&  p) console_write("User process tried to read a page and caused a protection fault!");
   if(us  &&  rw && !p) console_write("User process tried to write to a non-present page entry!");
   if(us  &&  rw &&  p) console_write("User process tried to write a page and caused a protection fault!");
   
   kprintf("\nAt %X \n", fault_addr);
   
   print_dalek();
   
   PANIC("page fault");
}

void setup_paging()
{
    // Our directories
    kernel_dir = 0; // The kernel's page directory
    current_dir = 0; // The current page directory;
    // Here we get the length of usable memory
    uint32_t end_of_page = (uint32_t) (_length + _addr); //_length and _addr is obtained during startup
    kprintf("End of kernel: %X", addrPtr); kprintf(" End of page: %X", end_of_page);
    // We get the amount of frames from the length
    nframes = (uint32_t) end_of_page / PAGE_SIZE;
    kprintf(" Number of page frames: %u", nframes);
    // We then allocate RAM for the page frames
    frames = (uint32_t *) h_kmalloc(INDEX_FROM_BIT(nframes), false, 0);
    // And clear it
    memset(frames, 0, INDEX_FROM_BIT(nframes));
    
    // Let's set up the kernel directory
    kernel_dir = (page_directory_t*) h_kmalloc(sizeof(page_directory_t), true, 0);
    memset(kernel_dir, 0, sizeof(page_directory_t));
    current_dir = kernel_dir;
    
    uint32_t i = 0;
    while(i < addrPtr) //While we are still within the kernel memory area
    {
        // Then identity map the pages that the kernel is in to make our lives easier (i < end_of_kernel)
        alloc_frame(getPage(i, true, kernel_dir), true, false);
        i += PAGE_SIZE;
    }
    idt_set_gate(14, (unsigned)page_fault, 0x08, 0x8E); // Install page fault handler
    load_page_dir(kernel_dir); // Load the directory
    enable_paging(); // Enable paging
    console_putc('\n'); bprintok(); console_writeline("Paging enabled!");
}