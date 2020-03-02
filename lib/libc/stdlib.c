/**
 * Copyright (c) 2019 The cxkernel Authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 *
 * @file   stdlib.c
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on June 04 2019, 10:34 AM
 */

#include <stdlib.h>
#include <panic.h>

void abort(void)
{
    PANIC("Kernel aborted!");
}

int abs(int j)
{
    return (j >= 0) ? j : -j;
}

long labs(long j)
{
    return (j >= 0) ? j : -j;
}

ldiv_t ldiv(long int n, long int d)
{
    ldiv_t res;
    res.quot = n / d;
    res.rem = n % d;
    if(n >= 0 && res.rem < 0)
    {
        res.quot++;
        res.rem -= d;
    }
    return res;
}

// TODO: This is really bad, change!!!!
static unsigned long next = 1;
int rand(void)
{
    next = next * 1103515245 + 12345;
    return((unsigned)(next/65536) % 32768);
}
