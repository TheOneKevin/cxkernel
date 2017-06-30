/*
 * File:   gdb.h
 * Author: Kevin
 *
 * Created on May 3, 2017, 5:30 PM
 */

#include "system.h"

#ifndef GDB_H
#define GDB_H

#ifdef __cplusplus
extern "C" {
#endif

void initDbg(uint32_t t);

void set_debug_traps();
void handle_exception(int exceptionVector);
void breakpoint();
bool isInited();

#ifdef __cplusplus
}
#endif

#endif /* GDB_H */
