/*
 * File:   bootacpi.c
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 2017-09-23T14:03:00-04:00
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2018-03-28T14:26:54-04:00
*/

#define __MODULE__ "_ACPI"

#include "lib/printk.h"
#include "lib/string.h"

#include "arch/x86/arch_common.h"
#include "arch/x86/acpi.h"
#include "arch/x86/acpitypes.h"

extern uint32_t _kernel_dir1;
extern uint32_t _kernel_table3;

// Pointers to tables
static ACPI_RSDP_t* currRSDP = NULL;
static ACPI_RSDT_t* RSDT = NULL;

// Stuff
static uint32_t _acpi_mmap_idx = 0;
static uint32_t _acpi_mmap_idx_fix = 0;

static bool doSDTChecksum(struct ACPI_SDT_Header* header)
{
    uint8_t* bptr = (uint8_t *) header;
    uint8_t sum = 0;
    for(uint32_t i = 0; i < header -> length; i++)
    {
        sum += *bptr;
        bptr++;
    }
    return sum == 0;
}

static ACPI_RSDP_t* checkValidRSDP(uint32_t* ptr)
{
    char* sig = "RSD PTR ";
    ACPI_RSDP_t* rsdp = (ACPI_RSDP_t *) ptr;
    uint8_t* bptr;
    uint8_t check = 0;

    // First check if the signature is valid
    if(memcmp(rsdp -> sig, sig, 8) == 0)
    {
        // Then check the checksum
        bptr = (uint8_t *) ptr;
        for(size_t i = 0; i < sizeof(ACPI_RSDP_t); i++)
        {
            check += *bptr;
            bptr++;
        }

        if(check == 0)
        {
            if(rsdp -> rev == 2)
            {
                OS_PRN("Detected ACPI Rev 2.x\n");
                // Check the checksum for the extended table
                ACPI_RSDP20_t* xptr = (ACPI_RSDP20_t *) ptr;
                bptr = (uint8_t *) (& (xptr -> len));
                for(size_t i = 0; i < sizeof(ACPI_RSDP20_t) - sizeof(ACPI_RSDP_t); i++)
                {
                    check += *bptr;
                    bptr++;
                }

                if(check == 0)
                    return (ACPI_RSDP_t *) xptr;
            }
            else if(rsdp -> rev == 0)
            {
                OS_PRN("Detected ACPI Rev 1.x\n");
                return rsdp;
            }
        }
    }
    return NULL;
}

static ACPI_RSDP_t* getRDSP(void)
{
    uint32_t* addr;
    ACPI_RSDP_t* rsdp;

    // Search under the 1 MB mark
    for(addr = (uint32_t *) ARCH_VIRT_PHYS(0xE0000); (uint32_t) addr < ARCH_VIRT_PHYS(0x100000); addr += 0x10 / sizeof(addr))
    {
        rsdp = checkValidRSDP(addr);
        if(rsdp != NULL)
            return rsdp;
    }

    uint32_t EBDA = *((uint8_t *) 0x40E);
    EBDA = EBDA * 0x10 & 0xFFFFF;

    // Loop through the EBDA
    for(addr = (uint32_t *) ARCH_VIRT_PHYS(EBDA); (uint32_t) addr < EBDA + 1024; addr += 0x10 / sizeof(addr))
    {
        rsdp = checkValidRSDP(addr);
        if(rsdp != NULL)
            return rsdp;
    }

    return NULL;
}

NO_OPTIMIZE static uint32_t mapACPItable(uint32_t addr, size_t len)
{
    uint32_t i = _acpi_mmap_idx_fix;
    for(uint32_t p = ARCH_PAGE_ALIGN(addr); p <= ARCH_PAGE_ALIGN(addr + len); p += ARCH_PAGE_SIZE)
    {
        uint32_t* page_table = &_kernel_table3;
        page_table[i] = addr | 0x3;
        i++;
        _acpi_mmap_idx = i;
    }

    // Flush TLB
    asm volatile("movl %cr3,%eax");
    asm volatile("movl %eax,%cr3");

    return ((addr - ARCH_PAGE_ALIGN_DOWN(addr))     // Offset within the page
                 + (ARCH_VIRT_BASE - 1024 * ARCH_PAGE_SIZE) // Address of (base pt - 1)
                 + (_acpi_mmap_idx_fix * ARCH_PAGE_SIZE));  // Offset from the pt
}

static void fixACPImmap(void)
{
    _acpi_mmap_idx_fix = _acpi_mmap_idx;
}

NO_OPTIMIZE static struct ACPI_SDT_Header* findTable(char* name)
{
    int entries = (RSDT -> head.length - sizeof(RSDT -> head)) / sizeof(RSDT -> pointers);
    for(int i = 0; i < entries; i++)
    {
        struct ACPI_SDT_Header* table = (struct ACPI_SDT_Header *) mapACPItable(*(&RSDT -> pointers + i), sizeof(struct ACPI_SDT_Header));
        if(memcmp(table -> signature, name, 4) == 0)
        {
            table = (struct ACPI_SDT_Header *) mapACPItable(*(&RSDT -> pointers + i), table -> length);
            if(doSDTChecksum(table))
                return table;
        }
    }

    return NULL;
}

NO_OPTIMIZE void getNUMADomains(void)
{
    ACPI_SRAT_t* SRAT = (ACPI_SRAT_t *) findTable("SRAT");
    fixACPImmap();
    if(SRAT == NULL)
    {
        OS_PRN("[ACPI] No SRAT found. Assuming single NUMA domain.\n");
        return;
    }

    OS_PRN("Found\n");
}

NO_OPTIMIZE void getCPUInfo(void)
{
    ACPI_MADT_t* table = (ACPI_MADT_t *) findTable("APIC");
    fixACPImmap();
    if(table == NULL)
    {
        OS_PRN("[ACPI] No MADT found. APIC unsupported.\n");
        return;
    }

    uint32_t addr = ((uint32_t) table) + sizeof(ACPI_MADT_t);
    while(addr < (uint32_t) table + table -> head.length)
    {
        struct MADT_proc_lapic_struct* head = (struct MADT_proc_lapic_struct *) addr;
        if(head -> type == 0)
            OS_PRN("CPU %X %X %X found.\n", head -> proc_id, head -> apic_id, table -> controller_address);
        addr += head -> length;
    }
}

NO_OPTIMIZE bool acpiHasAPIC(void)
{
    return findTable("APIC") != NULL;
}

NO_OPTIMIZE void initTmpBootACPI(void)
{
    currRSDP = getRDSP();
    if(currRSDP == NULL)
    {
        OS_ERR("%s\n", "[ACPI boot] Cannot find RSDP!\n");
        return;
    }

    // Prepare the ACPI page table and page dir
    uint32_t* kernel_pt = &_kernel_table3;
    memset(kernel_pt, 0, sizeof(uint32_t) * 1024);
    uint32_t* page_dir = &_kernel_dir1;
    page_dir[ARCH_PAGE_NUMBER(ARCH_VIRT_BASE) - 1] = ((uint32_t) kernel_pt - ARCH_VIRT_BASE) + 0x3;

    // First map the header of the RSDT, then map the entire table using the length from the header
    struct ACPI_SDT_Header* _RSDT = (struct ACPI_SDT_Header*) mapACPItable(currRSDP -> rsdtAddr, sizeof(struct ACPI_SDT_Header));
    _RSDT = (struct ACPI_SDT_Header*) mapACPItable(currRSDP -> rsdtAddr, _RSDT -> length);
    RSDT = (ACPI_RSDT_t *) _RSDT;
    fixACPImmap();
    if(!doSDTChecksum(_RSDT) && memcmp(_RSDT -> signature, "RSDT", 4) != 0)
    {
        OS_ERR("%s\n", "[ACPI boot] Invalid RSDT!\n");
        return;
    }

    BOCHS_MAGIC_BREAK();
}

void deinitTmpBootACPI(void)
{
    currRSDP = NULL;
    // Disable the table
    uint32_t* page_dir = &_kernel_dir1;
    page_dir[ARCH_PAGE_NUMBER(ARCH_VIRT_BASE) - 1] = 0;
}
