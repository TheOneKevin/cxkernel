/*
 * File:   apic.c
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 2017-09-27T16:19:04-04:00
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2017-11-26T12:44:15-05:00
*/

#include "lib/printk.h"
#include "arch/x86/cpu.h"
#include "arch/x86/pic.h"
#include "arch/x86/apic.h"
#include "arch/x86/acpi.h"
#include "arch/x86/interrupts.h"

void enable_APIC(void)
{
    if(cpu_check_feat(CPU_FEAT_EDX_APIC, CPU_FEATSET_REGS_EDX))
    {
        // First try to get APIC base addr from MSR
        if(cpu_check_feat(CPU_FEAT_EDX_MSR, CPU_FEATSET_REGS_EDX))
        {

        }
        // Then try getting it from MADT
        else if(acpiHasAPIC())
        {

        }
        else
            goto APIC_fail;
    }

    else
    {
APIC_fail:
        fprintf(STREAM_ERR, "APIC is not supported. This is just a warning.\n");
        kprintf("Defaulting to PIC...\n");
        PIC_autoremap();
    }
}
