/**
 * @file   bitmap.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Sunday, November 26th 2017, 9:42:00 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-07-06T15:29:28-04:00
 */

#pragma once

#include "common.h"

typedef struct bitmap
{
    size_t length;        //!< Array size of bitmap, not total bit count
    unsigned int* bitmap; //!< Bitmap array
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
