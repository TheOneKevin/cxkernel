/**
 * Copyright (c) 2019 The cxkernel Authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 *
 * @file   interrupts.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on January 03 2019, 1:43 PM
 */

#pragma once

#include "common.h"

__BEGIN_CDECLS

typedef void (*irq_handler)(void*);
void irq_install_handler(int, irq_handler);
void irq_remove_handler(int);

__END_CDECLS

#ifdef __cplusplus
namespace platform
{
    class Irq
    {
    public:
        virtual void install_handler(int n, irq_handler handler);
        virtual void remove_handler(int n);
    };
}
#endif
