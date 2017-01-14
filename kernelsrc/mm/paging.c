/*
 * Auuuuuuauaaaaauauuauauauauugh! I hate paging. I hate paging. This is a nightmare. Here's a life lesson:
 * NEVER. EVER. USE. James Molloy's CODE. If you plan on using his code, don't. Use it as a REFERENCE. You can
 * Thank me later. Ugh.
 *
 * Also, decide beforehand whether you want a higher-half kernel or not!
 *
 * Some things to note whilst traversing the barren landscape: All addresses stored in the page directories are physical. This means that
 * we have to do some page frame -> physical conversion and virtual -> physical conversions by hand
 */

#include "memory/kheap.h"
#include "memory/paging.h"
#include "memory/pmm.h"

#include "system/PANIC.h"
#include "system/kprintf.h"

#include "arch/exceptions.h"

#include "display/tdisplay.h"
#include "drivers/vesa.h"

// Page directory entry format
//Name: P R U W D A 0 S G Av. Addr.
//Size: 1 1 1 1 1 1 1 1 1 3   20
// Page table entry format
//Name: P R U W C A D 0 G Av. Addr.
//Size: 1 1 1 1 1 1 1 1 1 3   20

// We got to remember that memory addresses works in bytes. So, 0x0 to 0x4000 will be 4096 bytes of RAM.
// We also know that each page directory OR page table ENTRY will be 4 BYTES LONG! Each table/directory will be
// 1024 entries long. So, the page directory AND page table will be 1024 * 4 = 4096 BYTES long each!! So, placing
// everything into RAM will require AT LEAST 0x400000 bytes, which is around 4 MB.

//So, we'll reserve the first MB for the kernel, the following 3 to heap, and 4 more to paging. Totaling 8 MB for kernel stuff.

uint64_t _length;
uint64_t _addr;

extern uint32_t _kernel_end;
vscreen_t vhscreen;

static bool paging_enabled = false;
static uint32_t* page_directory;
static uint32_t page_dir_ptr = 0;
static uint32_t* kpt;

inline void invlpg(void* m)
{
    // Clobber memory to avoid optimizer re-ordering access before invlpg, which may cause nasty bugs.
    asm volatile ("invlpg (%0)" : : "b"(m) : "memory");
}

/*uint32_t CNVRT_TOP(uint32_t addr) { return addr - KRNLBASE; }
uint32_t CNVRT_TOF(uint32_t addr) { return addr - FRAMEBSE + (uint32_t)&_kernel_end; }*/

uint32_t pgalgnup(const uint32_t addr)
{
    uint32_t a = addr;
    if(addr & 0xFFFFF000)
    {
        a &= 0xFFFFF000;
    }

    a = a < addr ? a + 0x1000 : a;

    return a;
}

//A constant used to find the physical address of the any given virtual frame address and vice versa
uint32_t CONSTFRAME()  { return FRAMEBSE - pgalgnup((uint32_t)&_kernel_end) + KRNLBASE - 0x400000; } //All operations reversed due to BEDMAS

status_t paging_map_virtual_to_phys(const uint32_t virt, const uint32_t phys, const uint32_t flags)
{
    //uint16_t pdid = pageAlign(virt) >> 22; //Get the page directory index from the virtual address
    //uint16_t ptid = pageAlign(virt) >> 12 & 0x03FF; //Page align the virtual address and get the index for the page table
    uint32_t v = pageAlign(virt);
    uint32_t pdid = v / 0x400000; //1024 table entries * 0x1000 per entry = size of 1 directory entry
    uint32_t ptid = (v / 0x1000) % 1024;
    uint32_t* pt = 0;

    if(page_directory[pdid] == 0) //If the page table does not exist
    {
        int a = pmm_allocFrame(1); //Allocate a frame to the page table itself
        //TODO: if(a < 0)

        uint32_t tmp = kpt[1023]; //Store the last entry of the KERNEL page table
        //Replace that entry with a new temporary mapping for the new tables location (physical address, that's why there's all the subtracting)
        kpt[1023] = ((a - CONSTFRAME()) | (flags & 0xFFF));

        page_directory[pdid] = kpt[1023]; //Add an entry in the page directory to point to that new mapped address + flags
        pt = (uint32_t *)a; //And set the pointer to point to that address
        kpt[1023] = tmp; //Reset the last entry of the kernel page table
    }

    pt = (uint32_t *)((page_directory[pdid] & ~0xFFF) + CONSTFRAME());

    if(pt[ptid] != 0)
    {
        //TODO: Uh oh, we need to overwrite something!
        return -ENOMEM;
    }
    pt[ptid] = pageAlign(phys) | (flags & 0xFFF) | 0x1; //Set the entry

    if(paging_enabled)
        invlpg(pt);
    return 0;
}

void paging_unalloc(uint32_t virt)
{
    uint32_t v = pageAlign(virt);
    uint32_t pdid = v / 0x400000; //1024 table entries * 0x1000 per entry = size of 1 directory entry
    uint32_t ptid = (v / 0x1000) % 1024;
    uint32_t* pt = 0;

    pt = (uint32_t *)((page_directory[pdid] & ~0xFFF) + CONSTFRAME());
    pt[ptid] = 0;

    if(paging_enabled)
        invlpg(pt);
}

uint32_t get_physaddr(uint32_t virt)
{
    uint32_t pdid = virt / 0x400000;
    uint32_t ptid = (virt / 0x1000) % 1024;

    if(page_directory[pdid] == 0)
        return -1; //Entry does not exist

    uint32_t* pt = (uint32_t *)(page_directory[pdid] & ~0xFFF);

    if(pt[ptid] == 0)
        return -1; //Entry does not exist
    //Something should be changed here
    return (pt[ptid] & ~0xFFF);
}

void paging_enable()
{
    asm volatile("mov %%eax, %%cr3": :"a"(page_dir_ptr - CONSTFRAME()));
    asm volatile("mov %cr0, %eax");
    asm volatile("orl $0x80000000, %eax");
    asm volatile("mov %eax, %cr0");
    paging_enabled = true;
}

void page_fault(regs_t *r)
{
    uint32_t fault_addr;
    //asm volatile("mov %%cr2, %0" : "=r" (fault_addr));
    fault_addr = read_cr2();

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

   PANIC("PAGE FAULT");
}

void paging_init()
{
    if(!vhscreen.enabled)
    {
        bprintinfo(); kprintf("Setting up paging\n");
    }

    page_directory = (uint32_t*)pmm_allocFrame(1);
    page_dir_ptr = (uint32_t)page_directory;
    kpt = (uint32_t *)pmm_allocFrame(1); //Let's allocate one frame for the kernel table right now
    memset(page_directory, 0, 1024); //Clear the page directory
    //Make the first page directory point to the kernel page table
    page_directory[0] = ((uint32_t)kpt - CONSTFRAME()) | 0x3;
    //We map all the memory from 0 to the start of the frames (kernel + kernel heap)
    for(uint32_t i = 0; i <= pgalgnup((uint32_t)&_kernel_end - KRNLBASE); i += 0x1000)
        paging_map_virtual_to_phys(i + KRNLBASE, i, 0b011);

    //Identity map the first MB + more for the sake of sanity of the kernel & stuff running in it
    for(uint32_t i = 0; i <= KRNLBASE; i += 0x1000)
        paging_map_virtual_to_phys(i, i, 0b011);

    for(uint32_t i = 0; i <= 0x800000; i += 0x1000)
        paging_map_virtual_to_phys(FRAMEBSE + i, i + (uint32_t)&_kernel_end - KRNLBASE + 0x401000, 0b011);

    for(uint32_t i = 0; i <= 0x400000; i += 0x1000)
        paging_map_virtual_to_phys(i + INRDBASE, (uint32_t)&_kernel_end - KRNLBASE + 0x1000 + i, 0b011);

    paging_map_virtual_to_phys(0xC03FF000, 0x000B8000, 0b011);

    if(vhscreen.enabled == true)
    {
        for(uint32_t i = getPixelAddr(0, 0); i <= getPixelAddr(vhscreen.width, vhscreen.height); i += 0x1000)
            paging_map_virtual_to_phys(i, i, 0b011);
    }

    //ASSERT(get_physaddr(0x1000) == 0x1000, "Memory improperly mapped!");
    paging_enable(); //Enable paging
    bprintok(); kprintf("Paging was successfully enabled!\n"); //Display that we are aye-ok
}
