/**
 * @file
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Wednesday, December 6th 2017, 10:28:21 am
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-08-05T14:49:42-04:00
 * 
 * \sa include/arch/x86/interface/arch_types.h
 * \sa include/arch/raspi/interface/arch_types.h
 * \sa include/arch/dummy/interface/arch_types.h
 */

#pragma once

#include "common.h"
#include STRINGIFY(arch/TARGET_FOLDER/interface/arch_types.h)

struct MEMORY_MAP
{
    virt_t KRN_MMAP;
    virt_t KRN_MMAP_END;
    virt_t KRN_PRIV_BEGIN;
    virt_t KRN_PRIV_END;
    virt_t KRN_BEGIN;
    virt_t KRN_END;
    virt_t KRN_BRK;
    virt_t KRN_SLIM;
    uint32_t KRN_STACK_SIZE;
    virt_t KRN_STACK_BEGIN;
    virt_t KRN_PAGE_TABLES_BEGIN;

    phys_t MOD_END;
};
