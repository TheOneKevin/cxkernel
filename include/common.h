/**
 * @file   common.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Thursday, October 11th 2018, 9:05:05 am
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-10-19T16:19:02-04:00
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
