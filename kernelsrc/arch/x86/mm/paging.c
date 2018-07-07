/*
 * File:   paging.c
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * @date Created on 03-Aug-2017 08:26:00 PM
 *
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-05-23T13:41:36-04:00
*/

#define __MODULE__ "PGING"

#include "arch/x86/global.h"

#include "lib/printk.h"
#include "lib/string.h"
#include "mm/page_alloc.h"

#include "arch/x86/cpu.h"
#include "arch/x86/llio.h"
#include "arch/x86/paging.h"
#include "arch/x86/arch_common.h"

extern uint32_t _kernel_dir1;
extern uint32_t _kernel_dir2;
extern uint32_t _kernel_dir3;
extern uint32_t _kernel_dir4;

extern uint32_t _kernel_table1; // Kernel image
extern uint32_t _kernel_table2; // Kenrel stack
// For PAE
extern uint32_t _kernel_table3; // Unused
extern uint32_t _kernel_table4; // Unused
extern uint32_t _kernel_table5; // Unused
extern uint32_t _kernel_table6; // Unused
extern uint32_t _kernel_table7; // Unused
extern uint32_t stack_bottom;

extern void enable_pae_asm(uint32_t);

// Pointers to the 4 page tables for PAE mapping structures
uint64_t* PAE_PTRS[4];

#define TO64(x) ((uint64_t) ((uint32_t) (x)))
#define TOP64(x) (TO64(x) - ARCH_VIRT_BASE)

static inline void __tlb_flush_all(void)
{
    // Read the value of CR3 into CR3 to flush the TLB
    write_cr3(read_cr3());
}

static inline void __tlb_flush_single(uint32_t addr)
{
   asm volatile("invlpg (%0)" ::"r" (addr) : "memory");
}

/* ============================================================================= */
/*                          NON PAE MODE CODE                                    */
/* ============================================================================= */

/**
 * Maps an entire page table in to the page table place thing
 * @param virt Virtual address the page table maps
 * @param phys Physical address of page table
 */
static inline void __map_pde(uint32_t virt, uint32_t phys)
{
    (&_kernel_table3)[ARCH_GET_PD_IDX(virt)] = phys | (PTE_PR | PTE_RW);
    __tlb_flush_single(g_memory_map.KRN_PAGE_TABLES_BEGIN + ARCH_PAGE_SIZE * ARCH_GET_PD_IDX(virt));
}

static int __map_page(virt_t virt, phys_t phys, uint16_t flags)
{

    int pdid = ARCH_GET_PD_IDX(virt);
    int ptid = ARCH_GET_PT_IDX(virt);
    uint32_t* page_dir = &_kernel_dir1;
    uint32_t* page_tab = (uint32_t *)(g_memory_map.KRN_PAGE_TABLES_BEGIN + ARCH_PAGE_SIZE * pdid);

    if(page_dir[pdid] == 0)
    {
        uint32_t addr = (uint32_t) pmm_alloc_page();
        __map_pde(virt, addr);
        memset(page_tab, 0, sizeof(uint32_t) * 1024);
        page_dir[pdid] = addr | (PDE_RW | PDE_PR);
    }

    //TODO: Page align
    page_tab[ptid] = ((uint32_t) phys) | (flags & 0xFFF);

    __tlb_flush_single(virt & ARCH_PAGE_MASK);
    return 0;
}

static phys_t __get_phys(virt_t virt)
{
    int pdid = ARCH_GET_PD_IDX(virt);
    int ptid = ARCH_GET_PD_IDX(virt);
    uint32_t* page_tab = (uint32_t *)(g_memory_map.KRN_PAGE_TABLES_BEGIN + ARCH_PAGE_SIZE * pdid);
    if((&_kernel_dir1)[pdid] == 0) // Address doesn't exist
    {
        errno = EFAULT;
        return 0;
    }

    return page_tab[ptid] & ARCH_PAGE_MASK;
}

static virt_t __get_virt(phys_t addr)
{
    errno = ENOSYS;
    return addr;
}

static void __init_paging(void)
{
    OS_PRN("%-66s", "Enabling x86 paging...");

    g_memory_map.KRN_PAGE_TABLES_BEGIN = 0xFFC00000;

    uint32_t* page_dir = &_kernel_dir1;
    uint32_t* page_tab = &_kernel_table3;

    // Remove identity map
    page_dir[0] = 0;

    // Map the page tables to their respective virtual addresses
    memset(page_tab, 0, 1024 * sizeof(uint32_t));
    page_tab[ARCH_GET_PD_IDX(ARCH_VIRT_BASE)] = ((uint32_t)(&_kernel_table1) - ARCH_VIRT_BASE) | (PTE_PR | PTE_RW);
    page_tab[ARCH_GET_PD_IDX(0xFF7FF000)] = ((uint32_t)(&_kernel_table2) - ARCH_VIRT_BASE) | (PTE_PR | PTE_RW);
    page_tab[1023] = ((uint32_t)(&_kernel_table3) - ARCH_VIRT_BASE) | (PTE_PR | PTE_RW);

    // Add it as an entry to the page directory
    page_dir[ARCH_GET_PD_IDX(g_memory_map.KRN_PAGE_TABLES_BEGIN)] = ((uint32_t)(&_kernel_table3) - ARCH_VIRT_BASE) | (PTE_PR | PTE_RW);

    // Flush the entire TLB
    __tlb_flush_all();

    // Assign the relevant function pointer
    arch_map_page = &__map_page;
    arch_get_virt = &__get_virt;
    arch_get_phys = &__get_phys;

    fprintf(STREAM_OUT, "DONE!\n");
}

/* ============================================================================= */
/*                              PAE MODE CODE                                    */
/* ============================================================================= */

/**
 * Maps an entire page table in to the page table place thing
 * @param virt Virtual address the page table maps
 * @param phys Physical address of page table
 */
static inline void __map_pde_pae(uint32_t virt, uint64_t phys)
{
    (PAE_PTRS[ARCH_PAE_GET_PDPT_IDX(virt)])[ARCH_PAE_GET_PD_IDX(virt)] = phys | (PTE_PR | PTE_RW);
    __tlb_flush_single(g_memory_map.KRN_PAGE_TABLES_BEGIN + ARCH_PAGE_SIZE * (ARCH_PAE_GET_PDPT_IDX(virt) * 512 + ARCH_PAE_GET_PD_IDX(virt)));
}

static int __map_page_pae(virt_t virt, phys_t phys, uint16_t flags)
{
    int pdpt = ARCH_PAE_GET_PDPT_IDX(virt);
    int pdid = ARCH_PAE_GET_PD_IDX(virt);
    int ptid = ARCH_PAE_GET_PT_IDX(virt);
    uint64_t* page_dir = (uint64_t *)(((uint32_t) g_paging_PDPT[pdpt] & ARCH_PAGE_MASK) + ARCH_VIRT_BASE);
    uint64_t* page_tab = (uint64_t *)(g_memory_map.KRN_PAGE_TABLES_BEGIN + ARCH_PAGE_SIZE * (pdpt * 512 + pdid));
    if(page_dir[pdid] == 0)
    {
        uint64_t addr = pmm_alloc_page();
        __map_pde_pae(virt, addr);
        memset(page_tab, 0, sizeof(uint64_t) * 512);
        page_dir[pdid] = addr | (PDE_RW | PDE_PR);
    }

    //TODO: Page align
    page_tab[ptid] = (uint64_t)(((uint64_t) phys) | ((uint64_t) flags & 0xFFF));
    __tlb_flush_single(virt & ARCH_PAGE_MASK);
    return 0;
}

static virt_t __get_virt_pae(phys_t addr)
{
    errno = ENOSYS;
    return addr;
}

static phys_t __get_phys_pae(virt_t virt)
{
    int pdpt = ARCH_PAE_GET_PDPT_IDX(virt);
    int pdid = ARCH_PAE_GET_PD_IDX(virt);
    int ptid = ARCH_PAE_GET_PT_IDX(virt);
    uint64_t* page_dir = (uint64_t *)(((uint32_t) g_paging_PDPT[pdpt] & ARCH_PAGE_MASK) + ARCH_VIRT_BASE);
    uint64_t* page_tab = (uint64_t *)(g_memory_map.KRN_PAGE_TABLES_BEGIN + ARCH_PAGE_SIZE * (pdpt * 512 + pdid));
    if(page_dir[pdid] == 0)  // Address doesn't exist
    {
        errno = EFAULT;
        return 0;
    }

    return page_tab[ptid] & ARCH_PAGE_MASK_LL;
}

static void __init_paging_pae(void)
{
    OS_PRN("%-66s", "Enabling PAE mode paging...");

    memset(g_paging_PDPT, 0, 4 * sizeof(uint64_t));

    g_memory_map.KRN_PAGE_TABLES_BEGIN = 0xFF800000;

    // Kernel mappings
    uint64_t* page_dir1 = (uint64_t *)(&_kernel_table3);
    uint64_t* page_tab1 = (uint64_t *)(&_kernel_table4);
    uint64_t* page_tab2 = (uint64_t *)(&_kernel_table5);
    // And stack
    uint64_t* page_tab3 = (uint64_t *)(&_kernel_table6);
    // And identity map
    uint64_t* page_dir2 = (uint64_t *)(&_kernel_table7);

    // Map the kernel
    memset(&_kernel_table3, 0, 512 * sizeof(uint64_t));
    memset(&_kernel_table4, 0, 512 * sizeof(uint64_t));
    memset(&_kernel_table5, 0, 512 * sizeof(uint64_t));
    memset(&_kernel_table6, 0, 512 * sizeof(uint64_t));
    memset(&_kernel_table7, 0, 512 * sizeof(uint64_t));

    // Fill in the page tables (map from 0x000000 - 0x3FFFFF to ARCH_VIRT_BASE)
    for(uint64_t i = 0; i < 512; i++)
    {
        page_tab1[i] = (i * ARCH_PAGE_SIZE) | (PTE_RW | PTE_PR);
        page_tab2[i] = (0x200000L + i * ARCH_PAGE_SIZE) | (PTE_RW | PTE_PR);
    }

    g_paging_PDPT[ARCH_PAE_GET_PDPT_IDX(ARCH_VIRT_BASE)] = (TOP64(page_dir1)) | PDP_PR;
    // We can map the entire kernel using 2 PAE page tables
    page_dir1[ARCH_PAE_GET_PD_IDX(ARCH_VIRT_BASE)]     = (TOP64(page_tab1)) | (PDE_RW | PDE_PR);
    page_dir1[ARCH_PAE_GET_PD_IDX(ARCH_VIRT_BASE) + 1] = (TOP64(page_tab2)) | (PDE_RW | PDE_PR);
    // In fact, the kernel and stack shares the same page directory
    page_dir1[ARCH_PAE_GET_PD_IDX(0xFF7FF000)] = (TOP64(page_tab3)) | (PDE_RW | PDE_PR);
    page_tab3[ARCH_PAE_GET_PT_IDX(0xFF7FF000)] = (TOP64(&stack_bottom)) | (PTE_RW | PTE_PR);
    // However, the identity map does not. Identity map solely for the purpose of enabling PAE (seems almost pointless huh?)
    g_paging_PDPT[ARCH_PAE_GET_PDPT_IDX(0x100000)] = (TOP64(page_dir2)) | PDP_PR;
    page_dir2[ARCH_PAE_GET_PDPT_IDX(0x100000)]     = (TOP64(page_tab1)) | (PDE_RW | PDE_PR);
    page_dir2[ARCH_PAE_GET_PDPT_IDX(0x300000) + 1] = (TOP64(page_tab2)) | (PDE_RW | PDE_PR);

    // Enable PAE
    enable_pae_asm((uint32_t) g_paging_PDPT - ARCH_VIRT_BASE);

    // Remove identity map
    g_paging_PDPT[0] = 0;
    page_dir2[ARCH_PAE_GET_PDPT_IDX(0x100000)]     = 0;
    page_dir2[ARCH_PAE_GET_PDPT_IDX(0x300000) + 1] = 0;

    // Properly map the page tables now
    // We can use the 4 remaining unused pages to map all our page structures
    PAE_PTRS[0] = (uint64_t *)(&_kernel_dir1);
    PAE_PTRS[1] = (uint64_t *)(&_kernel_table1);
    PAE_PTRS[2] = (uint64_t *)(&_kernel_table2);
    PAE_PTRS[3] = page_dir2;

    // Clear the almost certainly used tables
    memset(&_kernel_dir1, 0, 512 * sizeof(uint64_t));
    memset(&_kernel_table1, 0, 512 * sizeof(uint64_t));
    memset(&_kernel_table2, 0, 512 * sizeof(uint64_t));

    (PAE_PTRS[ARCH_PAE_GET_PDPT_IDX(ARCH_VIRT_BASE)])[ARCH_PAE_GET_PD_IDX(ARCH_VIRT_BASE)] = (TOP64(page_tab1)) | (PTE_PR | PTE_RW);
    (PAE_PTRS[ARCH_PAE_GET_PDPT_IDX(ARCH_VIRT_BASE)])[ARCH_PAE_GET_PD_IDX(ARCH_VIRT_BASE) + 1] = (TOP64(page_tab2)) | (PTE_PR | PTE_RW);
    (PAE_PTRS[ARCH_PAE_GET_PDPT_IDX(0xFF7FF000)])[ARCH_PAE_GET_PD_IDX(0xFF7FF000)] = (TOP64(page_tab3)) | (PTE_PR | PTE_RW);

    (PAE_PTRS[3])[508] = (TOP64(PAE_PTRS[0])) | (PTE_PR | PTE_RW);
    (PAE_PTRS[3])[509] = (TOP64(PAE_PTRS[1])) | (PTE_PR | PTE_RW);
    (PAE_PTRS[3])[510] = (TOP64(PAE_PTRS[2])) | (PTE_PR | PTE_RW);
    (PAE_PTRS[3])[511] = (TOP64(PAE_PTRS[3])) | (PTE_PR | PTE_RW);

    page_dir1[508] = (TOP64(PAE_PTRS[0])) | (PTE_PR | PTE_RW);
    page_dir1[509] = (TOP64(PAE_PTRS[1])) | (PTE_PR | PTE_RW);
    page_dir1[510] = (TOP64(PAE_PTRS[2])) | (PTE_PR | PTE_RW);
    page_dir1[511] = (TOP64(PAE_PTRS[3])) | (PTE_PR | PTE_RW);

    // We can set up the PDPT for the kernel image and stack
    memset(&_kernel_dir2, 0, 512 * sizeof(uint64_t));
    memset(&_kernel_dir3, 0, 512 * sizeof(uint64_t));
    memset(&_kernel_dir4, 0, 512 * sizeof(uint64_t));
    g_paging_PDPT[0] = (TOP64(&_kernel_dir2)) | PDP_PR;
    g_paging_PDPT[1] = (TOP64(&_kernel_dir3)) | PDP_PR;
    g_paging_PDPT[2] = (TOP64(&_kernel_dir4)) | PDP_PR;

    // We don't need to map the 4 page directories as we assume they've been mapped to the kernel already
    // Flush the entire TLB
    __tlb_flush_all();

    // Assign the relevant function pointer
    arch_map_page = &__map_page_pae;
    arch_get_virt = &__get_virt_pae;
    arch_get_phys = &__get_phys_pae;

    fprintf(STREAM_OUT, "DONE!\n");
}

/* ============================================================================= */
/*                               INIT CODE                                       */
/* ============================================================================= */

void init_paging(void)
{
    // Check if PAE is enabled, then enable it
#ifdef ARCH_PAE_ENABLE
    if(cpu_check_feat(CPU_FEAT_EDX_PAE, CPU_FEATSET_REGS_EDX))
        __init_paging_pae();
    else
#endif
        __init_paging();
}
