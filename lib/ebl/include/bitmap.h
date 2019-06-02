/**
 * Copyright (c) 2018 The cxkernel Authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 *
 * @file   bitmap.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on December 22 2018, 10:55 PM
 */

#pragma once

#include "common.h"

__BEGIN_CDECLS

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

/**
 * Get's the number of entries that should be in the bitmap
 * @param L The number of bits the bitmap has
 * @return The number of entries that should be in the bitmap
 */
#define bitmap_getlength(L) ((L) / (sizeof(unsigned int) * 8) + 1)

/**
 * Get's the number of bits within the bitmap
 * @param L The number of entries the bitmap has
 * @return The number of bits that should be in the bitmap
 */
#define bitmap_getbits(L) ((L) * sizeof(unsigned int) * 8) // 8 bits per char

/**
 * Get's the first occurence of a zeroth bit
 * @param b The bitmap to search
 * @return The index of the bit. If none was found, returns -1
 */
static inline int bitmap_firstz(bitmap_t b)
{
    int _ptr = 0;
    for(size_t i = 0; i < b.length; i++)
    {
        if(b.bitmap[i] != ~0U)
        {
            _ptr = 0;
            while(_ptr < 32 && bitmap_tstbit(b.bitmap, _ptr + bitmap_getbits(i))) _ptr++;
            _ptr += (int) bitmap_getbits(i);
            return _ptr;
        }
    }
    return -1;
}

/**
 * Gets the last occurence of a zeroth bit
 * @param b The bitmap to search
 * @return The index of the bit. If none was found, returns -1
 */
static inline int bitmap_lastz(bitmap_t b)
{
    int _ptr = 0;
    for(size_t i = b.length; i > 0; i--)
    {
        if(b.bitmap[i - 1] != ~0U)
        {
            while(_ptr > 0 && bitmap_tstbit(b.bitmap, (_ptr - 1) + bitmap_getbits(i - 1))) _ptr--;
            _ptr += (int) bitmap_getbits(i - 1);
            return _ptr;
        }
    }
    return -1;
}

__END_CDECLS
