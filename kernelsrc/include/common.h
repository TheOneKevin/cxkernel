/**
 * @file   common.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Sunday, November 26th 2017, 9:42:00 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-07-06T19:08:53-04:00
 */

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include <stdarg.h>

#include "config.h"
#include "export.h"
#include "compiler_flags.h"
#include "utils.h"
#include "errno.h"

#define STREAM_OUT 0 //!< Same as stdout
#define STREAM_ERR 1 //!< Same as stderr
#define STREAM_LOG 2 //!< Not an std stream

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h> // C doesn't have booleans by default.

#ifdef __cplusplus
}
#endif
