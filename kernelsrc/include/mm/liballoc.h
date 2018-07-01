/*
 * File:   liballoc.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 * 
 * Created on Saturday, June 30th 2018, 11:25:41 pm
 * 
 * Last modified by:   Kevin Dai
 * Last modified time: 2018-06-30T23:49:34-04:00
 */

#include "arch/arch_types.h"

#define PREFIX(func) func

#ifdef __cplusplus
extern "C" {
#endif

void liballoc_init(uint32_t sz, uint32_t pc, phys_t (*a)(void), void (*b)(phys_t));

void    *PREFIX(malloc)(size_t);
void    *PREFIX(realloc)(void *, size_t);
void    *PREFIX(calloc)(size_t, size_t);
void     PREFIX(free)(void *);

#ifdef __cplusplus
}
#endif

#undef PREFIX