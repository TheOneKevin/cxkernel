/**
 * @file   utils.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Sunday, November 26th 2017, 9:42:00 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-07-05T20:47:15-04:00
 * 
 * General purpose macros goes here
 */

#pragma once

#define NEST(x) x
#define IN ,
#define OF ,

#define CHECK_FLAG(flags, bit) ((flags) & (1 << (bit)))
#define STRINGIFY_HIDDEN(x) #x
#define STRINGIFY(x) STRINGIFY_HIDDEN(x)

#define container_of(ptr, type, member) ({                  \
    const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
    (type *)( (char *)__mptr - offsetof(type,member) );})


// Yeah!