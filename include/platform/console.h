/**
 * Copyright (c) 2019 The cxkernel authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 *
 * @file   console.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on October 13 2018, 6:08 PM
 */

#pragma once
#include "common.h"

// NB: Included by printk.c
#ifdef __cplusplus
namespace platform
{
    class Console
    {
    public:
        virtual void log(char c);
        virtual void putc(char c);
        virtual void clear(void);
        virtual char getc(void);
    };
} // namespace platform
#endif

__BEGIN_CDECLS

void console_emit(char);
void console_puts(char*);
char console_getc(void);
void console_log(char*);

__END_CDECLS