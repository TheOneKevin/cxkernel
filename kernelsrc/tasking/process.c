/**
 * File:   process.c
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 * 
 * @date Created on Sunday, July 22nd 2018, 4:20:09 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-08-04T22:23:40-04:00
 */

//TODO: Seperate platform specific code

#define __MODULE__ "PROC "

#include "system.h"
#include "lib/string.h"
#include "mm/malloc.h"
#include "tasking/task.h"
#include "arch/x86/gdt.h"
#include "assert.h"
#include "lib/printk.h"

static list_t* process_queue = NULL;

extern uint32_t read_eip(void);

bool process_avail()
{
    return process_queue -> next != NULL;
}

void sched_process(process_t* proc)
{
    if(proc -> sched_node.next != NULL || proc -> sched_node.prev != NULL) return;
    list_add(&(proc -> sched_node), list_get_last(process_queue));
}

process_t* next_process()
{
    list_t* next = process_queue -> next;
    list_del(next);
    return LIST_ENTRY(next, process_t, sched_node);
}

void switch_task(virt_t ctx, bool resched)
{
    if(g_current_process == NULL) return; // Tasking has not been initialized yet
    if(!process_avail()) return; // Sched queue is empty
    g_current_process -> thread.regs.registers = (void *) ctx;
    g_current_process = next_process();
    //kprintf("%s\n", g_current_process -> name);
    if(resched) sched_process(g_current_process);
    asm volatile("movl %0, %%esp; jmp irq_return;" :: "r" (g_current_process -> thread.regs.registers));
}

void printA()
{
    while(true)
        fprintf(STREAM_ERR, "A");
}

void printB()
{
    while(true)
        fprintf(STREAM_ERR, "B");
}

process_t* create_task(virt_t entry_point)
{
    process_t* proc = (process_t *) malloc(sizeof(process_t));
    proc -> status = PROC_INIT;
    proc -> sched_node.next = NULL;
    proc -> sched_node.prev = NULL;

    uint32_t* stack = (uint32_t *) malloc(4096);
    stack = (uint32_t *)((int) stack + 4096);
    *--stack = 0x10;  // ss
    *--stack = 0x00;  // esp
    *--stack = 0x202; // eflags
    *--stack = 0x08;  // cs
    *--stack = entry_point; // eip
    *--stack = 0x00;  // error_code
    *--stack = 0x00;  // vec_no
    /* pushad */
    *--stack = 0xCAFEBABE; // eax
    *--stack = 0x00;  // ecx
    *--stack = 0x00;  // edx
    *--stack = 0x00;  // ebx
    *--stack = 0x00;  // esp_dummy
    *--stack = 0x00;  // ebp
    *--stack = 0x00;  // esi
    *--stack = 0x00;  // edi
    /* Push segment registers */
    *--stack = 0x10;  // ds
    *--stack = 0x10;  // es
    *--stack = 0x10;  // fs
    *--stack = 0x10;  // gs
    proc -> thread.regs.registers = (void *) stack;
    ASSERT_HARD(((regs_t*)proc -> thread.regs.registers) -> eax == 0xCAFEBABE, "iT FaILz");

    return proc;
}

void init_tasking(void)
{
    IRQ_OFF;
    if(process_queue != NULL)
    {
        OS_ERR("Tasking already installed.\n");
        IRQ_RES;
        return;
    }
    process_queue = create_list();
    process_t* init = (process_t *) malloc(sizeof(process_t));
    init -> name = strdup("init");
    init -> desc = strdup("Init process");
    init -> id = 0;
    init -> ppid = 0;
    init -> status = PROC_INIT;
    init -> sched_node.next = NULL;
    init -> sched_node.prev = NULL;

    process_t* procA = create_task((uint32_t) printA);
    procA -> name = strdup("A");
    procA -> desc = strdup("Just a process");
    procA -> id = 1;
    procA -> ppid = 0;
    process_t* procB = create_task((uint32_t) printB);
    procB -> name = strdup("A");
    procB -> desc = strdup("Just a process");
    procB -> id = 1;
    procB -> ppid = 0;

    sched_process(init);
    sched_process(procA);
    sched_process(procB);

    g_current_process = init;
    OS_PRN("Tasking installed.\n");
    
    IRQ_RES;
}