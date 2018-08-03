/**
 * File:   process.c
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 * 
 * @date Created on Sunday, July 22nd 2018, 4:20:09 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-08-03T19:46:51-04:00
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

void switch_task(bool resched)
{
    if(g_current_process == NULL) return; // Tasking has not been initialized yet
    if(!process_avail()) return; // Sched queue is empty
    g_current_process = next_process();

    //kprintf("%s\n", g_current_process -> name);

    if(resched) sched_process(g_current_process);
}

void printHi()
{
    fprintf(STREAM_ERR, "Hi\n");
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
    init -> name = strdup("krnl");
    init -> desc = strdup("Kernel process");
    init -> id = 0;
    init -> ppid = 0;
    init -> sched_node.next = NULL;
    init -> sched_node.prev = NULL;
    /*uint32_t kernel_stack, new_stack;
    new_stack = (uint32_t) malloc(4096);
    new_stack = new_stack + 4096;
    asm volatile("movl %%esp, %0; movl %1, %%esp; movl %1, %%ebp"
        : "=r" (kernel_stack) : "r" (new_stack));*/
    sched_process(init);
    g_current_process = init;
    OS_PRN("Tasking installed.\n");
    IRQ_RES;
}