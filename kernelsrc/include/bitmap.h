/*
 * File:   bitmap.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 2017-10-06T09:54:34-04:00
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2017-11-22T15:53:15-05:00
*/

#pragma once

#include "common.h"

typedef struct bitmap
{
    size_t length;
    unsigned int* bitmap;
} bitmap_t;

/**
 * Sets a bit in the bitmap
 * @param  A The bitmap array
 * @param  k The kth bit to set
 */
#define bitmap_setbit(A, k) (A[((k) / (sizeof(unsigned int) * 8))] |= (1 << ((k) % (sizeof(unsigned int) * 8))))

/**
 * Clears a bit in the bitmap
 * @param  A The bitmap array
 * @param  k The kth bit to clear
 */
#define bitmap_clrbit(A, k) (A[((k) / (sizeof(unsigned int) * 8))] &= ~(1 << ((k) % (sizeof(unsigned int) * 8))))

/**
 * Tests a bit in the bitmap
 * @param  A A bitmap array
 * @param  k The kth bit to test
 * @return   1 if set and 0 if not set
 */
#define bitmap_tstbit(A, k) (A[((k) / (sizeof(unsigned int) * 8))] & (1 << ((k) % (sizeof(unsigned int) * 8))))
