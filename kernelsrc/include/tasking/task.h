/**
 * @file   task.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Monday, July 2nd 2018, 4:03:00 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-07-30T12:43:42-04:00
 */

#pragma once

#include "common.h"
#include "list.h"
#include "tree.h"
#include "arch/arch_types.h"

typedef   signed int  pid_t;
typedef unsigned int  usr_t;
typedef   signed int  group_t;
typedef unsigned char status_t;

typedef struct thread
{
    virt_t          kernel_stack;
    virt_t          user_stack;
    struct PT_REGS  regs;
    uintptr_t*      page_directory;
} thread_t;

typedef struct fd_table {
    
} fd_table_t;

typedef struct process
{
    pid_t               id;         //!< Process ID (pid)
    pid_t               ppid;       //!< Parent PID

    char*               name;       //!< Process name
    char*               desc;       //!< Process description

    struct thread       thread;     //!< Task specific information (i.e., registers etc.)
    struct fd_table*    file_table; //!< File descriptor table

    list_t              sched_node; //!< Scheduler list node
    tree_node_t         ptree_node; //!< Process tree node
} process_t;

#ifdef __cplusplus
extern "C" {
#endif

void switch_task(bool resched);
void switch_next(void);
void init_tasking(void);

#ifdef __cplusplus
}
#endif