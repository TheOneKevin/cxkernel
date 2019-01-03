/**
 * Copyright (c) 2019 The cxkernel authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 *
 * @file   platform.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on October 13 2018, 6:24 PM
 */

#pragma once

#include "common.h"
#include "platform/console.h"
#include "platform/interrupts.h"

#ifdef __cplusplus
namespace platform
{
    Console& get_console(void);
    Irq& get_irq();
    void early_init(void);
    void init(void);
}
#endif
