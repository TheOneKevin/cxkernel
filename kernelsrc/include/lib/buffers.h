/*
 * File:   buffers.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 30-Jul-2017 08:49:41 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 31-Jul-2017 11:20:18 AM
*/

#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// Linear buffer data structure
typedef struct
{
    void*  data; // Pointer to data
    size_t size; // Size of data
} lbuffer_t;

#ifdef __cplusplus
}
#endif

// C++ code goes here
