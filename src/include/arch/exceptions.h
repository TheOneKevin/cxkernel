/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   exceptions.h
 * Author: Kevin
 *
 * Created on August 25, 2016, 11:02 AM
 */

#include "common.h"

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct registers
{
    unsigned int gs, fs, es, ds;      /* pushed the segs last */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    unsigned int int_no, err_code;    /* our 'push byte #' and ecodes do this */
    unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
} regs_t;

//Everything is reversed WHAT HAPPENED?!
//I saved this struct in case everything goes awry again :)
typedef struct rregisters
{
    unsigned int ds, es, fs, gs;
    unsigned int eax, ecx, edx, ebx, esp, ebp, esi, edi;
    unsigned int err_code, int_no;
    unsigned int ss, useresp, eflags, cs, eip;
} rregs_t;

void print_dalek();

#ifdef __cplusplus
}
#endif

#endif /* EXCEPTIONS_H */

