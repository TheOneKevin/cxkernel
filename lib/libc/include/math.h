/**
 * Copyright (c) 2018 The cxkernel Authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 *
 * @file   math.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on December 24 2018, 10:15 PM
 */

// We use old fashion include guards as math.h and algorithm.h are incompatible
#ifndef LIBC_MATH_H
#define LIBC_MATH_H
#ifdef EBL_ALGORITHM_H
#pragma GCC warning "ebl/include/algorithm.h and libc/include/math.h are incompatible.";
#endif

#include "common.h"

// Find maximum of a and b, avoids double evaluation too.
#define MAX(a,b) ({ TYPEOF (a) _a = (a); TYPEOF (b) _b = (b); _a > _b ? _a : _b; })

// Find minimum of a and b, avoids double evaluation too.
#define MIN(a,b) ({ TYPEOF (a) _a = (a); TYPEOF (b) _b = (b); _a < _b ? _a : _b; })

#define PI                  3.14159265
#define RAD2DEG(x)          ((x)/PI*180)
#define DEG2RAD(x)          ((x)*PI/180)
#define ALIGNB(x, align)    (((x) + ((align) - 1)) & ~((align) - 1))
#define ALIGN(x, align)     ((((x) + ((align) - 1)) / (align)) * (align))
#define FLOORB(x, align)    ((x) & ~((align) - 1))
#define FLOOR(x, align)     (((x) / (align)) * (align))
#define CEILB(x, align)     ALIGNB(x, align)
#define CEIL(x, align)      ALIGN(x, align)
#define CLIP(x, min, max)   (((x) < (min)) ? (min) : \
                            (((x) > (max)) ? (max) : (x)))
#define UCLIP(x, max)       (((x) > (max)) ? (max) : (x))
#define LCLIP(x, min)       (((x) < (min)) ? (min) : (x))
#define ABS(x)              (((x) <  0) ? -(x) : (x))
#define DIFF(a,b)           ABS((a)-(b))
#define IS_NAN(x)            ((x) != (x))
#define IMPLIES(x, y)       (!(x) || (y))
#define SWAP(a, b)          do { a ^= b; b ^= a; a ^= b; } while ( 0 )
#define SORT(a, b)          do { if ((a) > (b)) SWAP((a), (b)); } while (0)
#define COMPARE(x, y)       (((x) > (y)) - ((x) < (y)))
#define SIGN(x)             COMPARE(x, 0)
#define IS_ODD( num )       ((num) & 1)
#define IS_EVEN( num )      (!IS_ODD( (num) ))
#define IS_BETWEEN(n,L,H)   ((unsigned char)((n) >= (L) && (n) <= (H)))

#endif
