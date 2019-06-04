/**
 * Copyright (c) 2019 The cxkernel Authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 *
 * @file   strace.cc
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on August 13 2017, 3:29 PM
 */

#include <stdio.h>
#include <elf_parser.h>
#include "arch/x86/strace.h"
#include "arch/interface.h"
#include "arch/x86/global.h"
#include "arch/x86/arch_utils.h"

void CallStackTrace(arch_sz_t aEBP, bool aBOOL)
{
    auto* ebp = (arch_sz_t *) aEBP;
    if(aBOOL)
        ebp = &aEBP - 2;
    int i = 0;
    printf("StackTrace (ebp): 0x%X\n", ebp);
    while((arch_sz_t) ebp < ARCH_STACK_BOTTOM && (arch_sz_t) ebp > ARCH_STACK_BOTTOM - ARCH_PAGE_SIZE)
    {
        arch_sz_t eip = ebp[1];
        arch_sz_t sym_addr = elf::get_symbol_addr32(eip, x86::g::ctx);
        char* sym_name = elf::get_symbol_name32(eip, x86::g::ctx);
        if(sym_addr < ARCH_VIRT_BASE) sym_name = (char*) "???";
        if(i == 0)
            printf("Call generated from 0x%08X - %s() + %d\n", sym_addr, sym_name, eip - sym_addr);
        else
            printf("  (stacktrace) from 0x%08X - %s() + %d\n", sym_addr, sym_name, eip - sym_addr);
        ebp = (arch_sz_t *) (ebp[0]);
        i++;
    }
}