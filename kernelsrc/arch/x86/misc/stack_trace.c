/*
 * File:   stack_trace.c
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 13-Aug-2017 03:29:09 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2018-03-28T14:27:55-04:00
*/

#include "lib/printk.h"

#include "arch/x86/global.h"
#include "arch/x86/arch_common.h"
#include "arch/x86/stack_trace.h"

void CallStackTrace(uint32_t aEBP, bool aBOOL)
{
    uint32_t* ebp = (uint32_t *) aEBP;
    if(aBOOL)
        ebp = &aEBP - 2;
    int i = 0;
    kprintf("StackTrace (ebp): 0x%X\n", ebp);
    while((uint32_t) ebp < ARCH_STACK_TOP && (uint32_t) ebp > ARCH_STACK_TOP - ARCH_PAGE_SIZE)
    {
        uint32_t eip = ebp[1];
        uint32_t sym_addr = get_symbol_addr(eip, g_krnl_sym_table);
        if(i == 0)
            kprintf("Call generated from 0x%08X - %s() + %d\n", sym_addr, get_symbol_name(eip, g_krnl_sym_table), eip - sym_addr);
        else
            kprintf("  (stacktrace) from 0x%08X - %s() + %d\n", sym_addr, get_symbol_name(eip, g_krnl_sym_table), eip - sym_addr);
        ebp = (uint32_t *) (ebp[0]);
        i++;
    }
}
