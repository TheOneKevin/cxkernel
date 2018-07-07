/**
 * @file   acpi.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Sunday, November 26th 2017, 9:42:00 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-07-05T20:37:55-04:00
 */

#pragma once

#include "common.h"

typedef struct RSDPDescriptor
{
    char sig[8];
    uint8_t checksum;
    char OEMID[6];
    uint8_t rev;
    uint32_t rsdtAddr;
} PACKED ACPI_RSDP_t;

typedef struct RSDPDescriptor20
{
    ACPI_RSDP_t desc; //!< Top half of the table
    // Extended table
    uint32_t len;
    uint64_t XSDTAddr;
    uint8_t extChecksum;
    uint8_t res[3];
} PACKED ACPI_RSDP20_t;

struct ACPI_SDT_Header
{
    char signature[4];   //!< Contains signature
    uint32_t length;     //!< Length of entire table including entries
    uint8_t  rev;        //!< Revision number
    uint8_t  checksum;   //!< Entire table must sum to zero

    uint8_t  OEMID[6];   //!< What do <i>you</i> think this is?
    uint64_t OEMTableID; //!< I honestly don't know
    uint32_t OEMRev;     //!< OEM revision for OEM Table ID
    uint32_t CreatorID;  //!< Vendor ID of the utility used to create the table
    uint32_t CreatorRev; //!< Blah blah
} PACKED;

typedef struct RSDT
{
    struct ACPI_SDT_Header head;
    uint32_t pointers;
} PACKED ACPI_RSDT_t;

typedef struct SRAT
{
    struct ACPI_SDT_Header head;
    uint8_t reserved[12];
} PACKED ACPI_SRAT_t;

typedef struct MADT
{
    struct ACPI_SDT_Header head;
    uint32_t controller_address;
    uint32_t flags;
} PACKED ACPI_MADT_t;

/* =================== Entry Types ====================== */

struct MADT_proc_lapic_struct
{
    uint8_t type;
    uint8_t length;
    uint8_t proc_id;
    uint8_t apic_id;
    uint32_t flags;
} PACKED;

struct SRAT_proc_lapic_struct
{
    uint8_t type;      //!< 0x0 for this type of structure
    uint8_t length;    //!< 16
    uint8_t lo_DM;     //!< Bits [0:7] of the proximity domain
    uint8_t APIC_ID;   //!< Processor's APIC ID
    uint32_t flags;    //!< Haha the most useless thing ever
    uint8_t SAPIC_EID; //!< The processor's local SAPIC EID. Don't even bother.
    uint8_t hi_DM[3];  //!< Bits [8:31] of the proximity domain
    uint32_t _CDM;     //!< The clock domain which the processor belongs to (more jargon)
} PACKED;

struct SRAT_mem_struct
{
    uint8_t type;         //!< 0x1 for this type of structure
    uint8_t length;       //!< 40
    uint32_t domain;      //!< The domain to which this memory region belongs to
    uint8_t reserved1[2]; //!< Reserved
    uint32_t lo_base;     //!< Low 32 bits of the base address of the memory range
    uint32_t hi_base;     //!< High 32 bits of the base address of the memory range
    uint32_t lo_length;   //!< Low 32 bits of the length of the range
    uint32_t hi_length;   //!< High 32 bits of the length
    uint8_t reserved2[4]; //!< Reserved
    uint32_t flags;       //!< Flags
    uint8_t reserved3[8]; //!< Reserved
} PACKED;

struct SRAT_proc_lapic2_struct
{
    uint8_t type;         //!< 0x2 for this type of structure
    uint8_t length;       //!< 24
    uint8_t reserved1[2]; //!< Must be zero
    uint32_t domain;      //!< The proximity domain which the logical processor belongs to
    uint32_t x2APIC_ID;   //!< Processor's x2APIC ID
    uint32_t flags;       //!< Haha the most useless thing ever
    uint32_t _CDM;        //!< The clock domain which the processor belongs to (more jargon)
    uint8_t reserved2[4]; //!< Reserved.
} PACKED;

#ifdef __cplusplus
extern "C" {
#endif

void initTmpBootACPI(void) NO_OPTIMIZE;
void deinitTmpBootACPI(void);
void getNUMADomains(void) NO_OPTIMIZE;
void getCPUInfo(void) NO_OPTIMIZE;
bool acpiHasAPIC(void) NO_OPTIMIZE;

#ifdef __cplusplus
}
#endif
