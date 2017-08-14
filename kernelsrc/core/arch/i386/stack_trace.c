/*
 * Filename: stack_trace.c
 * Author:   Kevin Dai
 * Email:    kevindai02@outlook.com
 *
 * Created on 13-Aug-2017 03:29:09 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 13-Aug-2017 05:44:28 PM
*/

#include "lib/printk.h"

#include "arch/i386/common.h"
#include "arch/i386/sys/stack_trace.h"

extern int stack_top, stack_bottom;

void CallStackTrace(uint32_t aEBP, bool aBOOL)
{
    uint32_t* ebp = (uint32_t *) aEBP;
    if(aBOOL)
        ebp = &aEBP - 2;
    int i = 0;
    while((uint32_t) ebp < (uint32_t) &stack_top && (uint32_t) ebp > (uint32_t) &stack_bottom)
    {
        uint32_t eip = ebp[1];
        uint32_t sym_addr = get_symbol_addr(eip, krnl_sym_table);
        if(i == 0)
            kprintf("Call generated from 0x%08X - %s() + %d\n", sym_addr, get_symbol_name(eip, krnl_sym_table), eip - sym_addr);
        else
            kprintf("  (stacktrace) from 0x%08X - %s() + %d\n", sym_addr, get_symbol_name(eip, krnl_sym_table), eip - sym_addr);
        ebp = (uint32_t *) (ebp[0]);
        i++;
    }
}
