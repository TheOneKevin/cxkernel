/*
 * File:   utils.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 2017-11-12T15:46:28-05:00
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2017-11-26T14:42:02-05:00
*/

#pragma once

/* General purpose macros goes here */

#define NEST(x) x
#define IN ,
#define OF ,

#define CHECK_FLAG(flags, bit) ((flags) & (1 << (bit)))
#define STRINGIFY_HIDDEN(x) #x
#define STRINGIFY(x) STRINGIFY_HIDDEN(x)

#define container_of(ptr, type, member) ({                  \
    const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
    (type *)( (char *)__mptr - offsetof(type,member) );})
