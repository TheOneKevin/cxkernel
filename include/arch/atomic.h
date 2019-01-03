/**
 * Copyright (c) 2019 The cxkernel authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 *
 * @file   atomic.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on November 26 2017, 9:42 PM
 *
 * \sa include/arch/x86/interface/atomic.h
 * \sa include/arch/raspi/interface/atomic.h
 * \sa include/arch/dummy/interface/atomic.h
 *
 */

#pragma once

#include "common.h"
#include STRINGIFY(arch/TARGET_FOLDER/interface/arch_atomic.h)
