/**
 * @file   compiler_flags.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Sunday, November 26th 2017, 9:42:00 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-08-04T10:19:04-04:00
 */

#pragma once

#define NO_OPTIMIZE OPTIMIZE(0)
#define OPTIMIZE(x) __attribute__((optimize("O"#x)))
#define PACKED __attribute__((packed))
#define NOINLINE __attribute__((noinline))
#define ALIGNED(x) __attribute__((aligned(x)))
#define NORETURN __attribute__((noreturn))