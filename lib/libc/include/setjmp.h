/**
 * Copyright (c) 2019 The cxkernel Authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 * 
 * @file   setjmp.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on December 31 2019, 5:14 PM
 */

#pragma once

#include "common.h"

// TODO: Make architecture specific?

typedef intptr_t jmp_buf[40];
#define longjmp(buf, val) __builtin_longjmp(buf, val)
#define setjmp(buf) __builtin_setjmp(buf)
