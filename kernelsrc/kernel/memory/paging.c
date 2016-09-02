/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "memory/paging.h"

// Our frames bitset/bitmap
uint32_t *frames;
uint32_t nframes;
// Our directories
page_directory_t *kernel_dir = 0; // The kernel's page directory
page_directory_t *current_dir = 0; // The current page directory;

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
static void test_frame(uint32_t frame_addr)
{
    uint32_t frame  = frame_addr / PAGE_SIZE;
    uint32_t index  = INDEX_FROM_BIT(frame);
    uint32_t offset = OFFSET_FROM_BIT(frame);
    return (frames[index] & (0x1 << offset));
}

static uint32_t first_free_frame()
{
    //TODO: Keep a pointer to the last *successful* allocation
    //to greatly improve search time, as we know that all previous
    //frames have been taken
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
        return; //However, if there's no allocated frame, then return
    else
    {
        //Else, we free our frame
        clear_frame(frame);
        page->frame = 0x0; //And set the frame flag to 0 (not used)
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
   int present  = !(r -> err_code & 0x1); // Page not present
   int rw       = r -> err_code & 0x2;           // Write operation?
   int us       = r -> err_code & 0x4;           // Processor was in user-mode?
   int reserved = r -> err_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
   int id       = r -> err_code & 0x10;          // Caused by an instruction fetch?
   console_putc('\n');
   regdump(&r); //Dump regs
   // Output an error message.
   console_write("Page fault! (");
   if (present) {console_write("present");}
   if (rw) {console_write("read-only");}
   if (us) {console_write("user-mode");}
   if (reserved) {console_write("reserved");}
   if (id) { console_write("some instruction fetch error"); }
   console_write(") at 0x");
   console_write_hex(fault_addr);
   console_write("\n");
   PANIC("Page fault");
}

void setup_paging()
{
    uint32_t end_of_page = (uint32_t) (_length + _addr);
    //console_putc(' '); console_write_hex(end_of_page);
    nframes = (uint32_t) end_of_page / PAGE_SIZE;
    //console_putc(' '); console_write_dec(nframes);
    frames = (uint32_t *) h_kmalloc(INDEX_FROM_BIT(nframes), false, 0);
    memset(frames, 0, INDEX_FROM_BIT(nframes));
    
    kernel_dir = (page_directory_t*) h_kmalloc(sizeof(page_directory_t), true, 0);
    memset(kernel_dir, 0, sizeof(page_directory_t));
    current_dir = kernel_dir;
    
    int i = 0;
    while(i < addrPtr)
    {
        alloc_frame(getPage(i, true, kernel_dir), false, false);
        i += PAGE_SIZE;
    }
    idt_set_gate(14, (unsigned)page_fault, 0x08, 0x8E);
    load_page_dir(kernel_dir);
    enable_paging();
}