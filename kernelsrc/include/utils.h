/*
 * File:   utils.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 2017-11-12T15:46:28-05:00
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2018-04-03T20:38:07-04:00
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

/**
 * Returns the error status in a variable
 * passed down as a pointer to the function
 * @param  x The variable name in the parameter
 * @return   int** x
 */
#define ON_ERR(x) int* x
#define SET_ERR(x, y) *x = y;
#define NEW_EVAR(x) int x = 0;
#define EVAR_REF(x) &x
