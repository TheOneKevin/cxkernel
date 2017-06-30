/*
 * File:   task.h
 * Author: Kevin
 *
 * Created on June 22, 2017, 4:35 PM
*/

#ifndef TASK_H
#define TASK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

extern void initTasking();

typedef struct {
    uint32_t eax, ebx, ecx, edx, esi, edi, esp, ebp, eip, eflags, cr3;
} Registers;

typedef struct Task {
    Registers regs;
    struct Task *next;
} Task;

extern void initTasking();
extern void createTask(Task*, void(*)(), uint32_t, uint32_t*);

extern void preempt(); // Switch task frontend
extern void switchTask(Registers *old, Registers *new); // The function which actually switches

#ifdef __cplusplus
}
#endif

#endif /* _H */
