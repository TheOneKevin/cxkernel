/**
 * @file   config.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Thursday, October 11th 2018, 9:03:48 am
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-10-28T12:27:45-04:00
 */

#pragma once

#define ARCH_x86_32 1
#define ARCH_x86_64 2
#define CUSTOM_FILENAME
#define LOG_ENABLE
#ifdef LOG_ENABLE
    #define LOG_ALL_OUTPUT
#endif

// Include the autoconf generated header
#include "../build/config.h"
