/**
 * @file arch_interface.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Sunday, November 26th 2017, 9:42:00 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-11-13T20:30:16-05:00
 * 
 * \sa include/arch/x86/interface/arch_interface.h
 * \sa include/arch/raspi/interface/arch_interface.h
 * \sa include/arch/dummy/interface/arch_interface.h
 */

#pragma once

#include "common.h"
#include STRINGIFY(arch/TARGET_FOLDER/interface/arch_interface.h)

#ifndef ARCH_PAGE_SIZE
    #define ARCH_PAGE_SIZE
#endif
#ifndef ARCH_SYSCALL_INT_NO
    #define ARCH_SYSCALL_INT_NO
#endif
#ifndef ARCH_PAGE_ALIGN
    #define ARCH_PAGE_ALIGN
#endif
#ifndef ARCH_PAGE_ALIGN_DOWN
    #define ARCH_PAGE_ALIGN_DOWN
#endif
#ifndef ENABLE_INTERRUPTS
    #define ARCH_PAGE_ALIGN_DOWN
#endif
#ifndef DISABLE_INTERRUPTS
    #define DISABLE_INTERRUPTS
#endif

#ifdef __cplusplus

namespace arch
{
    void early_init(int, void*);
    void init(void);
}

#endif
