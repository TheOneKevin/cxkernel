/*
 * File:   paging.c
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 03-Aug-2017 08:26:00 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2017-11-26T12:47:08-05:00
*/

#include "lib/printk.h"
#include "lib/string.h"
#include "arch/x86/cpu.h"
#include "arch/x86/paging.h"

extern uint32_t _kernel_dir;
extern uint32_t _kernel_table1;
extern uint32_t _kernel_table2;
extern uint32_t _kernel_table3;

static void _init_paging_pae(void)
{

}

static void _init_paging(void)
{

}

void init_paging(void)
{
    memset(g_paging_PDPT, 0, 4 * sizeof(uint64_t));
    g_paging_PDPT[0] = (uint64_t)(&_kernel_dir);

    if(cpu_check_feat(CPU_FEAT_EDX_PAE, CPU_FEATSET_REGS_EDX))
    {

    }

    else
    {
        _init_paging();
    }
}
