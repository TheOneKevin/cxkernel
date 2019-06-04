/**
 * Copyright (c) 2019 The cxkernel Authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 *
 * @file ctype.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date Created on June 04 2019, 1:31 PM
 */

#pragma once

#ifndef libm
    #include "common.h"
#else
    #define __BEGIN_CDECLS
    #define __END_CDECLS
    #include <stdbool.h>
#endif

__BEGIN_CDECLS

int isalnum(int);
int isalpha(int);
int isascii(int);
int isblank(int);
int isdigit(int);
int islower(int);
int isspace(int);
int isupper(int);
int isxdigit(int);
int toascii(int);
int tolower(int);
int toupper(int);

__END_CDECLS
