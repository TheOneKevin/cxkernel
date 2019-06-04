/**
 * @file   stdlib.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Wednesday, October 10th 2018, 10:20:14 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-10-10T22:20:26-04:00
 */

#pragma once

#include "common.h"

/*
 * The Open Group Base Specifications Issue 6
 * IEEE Std 1003.1, 2004 Edition
 */

#define  RAND_MAX 32767

__BEGIN_CDECLS

typedef struct
{
    long int quot;  // Quotient.
    long int rem;   // Remainder.
} ldiv_t;

void          abort(void);
int           abs(int);
void          free(void *);
long          labs(long);
ldiv_t        ldiv(long, long);
void         *malloc(size_t);
int           rand(void);

__END_CDECLS