/*
 * File:   compiler_flags.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 2017-09-26T06:36:21-04:00
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2017-10-10T10:38:10-04:00
*/

#pragma once

#define NO_OPTIMIZE OPTIMIZE(0)
#define OPTIMIZE(x) __attribute__((optimize("O"#x)))
#define PACKED __attribute__((packed))
#define NOINLINE __attribute__((noinline))
#define ALIGNED(x) __attribute__((aligned(x)))
