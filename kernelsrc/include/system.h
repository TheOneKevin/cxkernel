/*
 * File:   system.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 29-Jul-2017 05:08:17 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 11-Aug-2017 05:33:46 PM
*/

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include <stdarg.h>

#include "sys/tty.h"

#define KERNEL_SELF_TEST 1

#define STREAM_NORM 0
#define STREAM_ERR  1
#define STREAM_INFO 2

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h> // C doesn't have booleans by default.

tty_t* _ttys[5];
uint8_t _current_tty_id;

#ifdef __cplusplus
}
#endif
