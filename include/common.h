/**
 * Copyright (c) 2019 The cxkernel authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 *
 * @file   common.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on October 11 2018, 9:05 AM
 */

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

#include "config.h"
#include "compiler.h"

#define STREAM_OUT 0 //!< Same as stdout
#define STREAM_ERR 1 //!< Same as stderr
#define STREAM_LOG 2 //!< Not an std stream

#ifndef __cplusplus
    #include <stdbool.h> // C doesn't have booleans by default.
#endif
