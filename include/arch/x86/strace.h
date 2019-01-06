/**
 * Copyright (c) 2019 The cxkernel Authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 *
 * @file   strace.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on January 05 2019, 10:35 AM
 */

#pragma once

#include "common.h"
#include "arch/x86/interface/arch_types.h"

__BEGIN_CDECLS

void CallStackTrace(arch_sz_t aEBP, bool aBOOL);

__END_CDECLS
