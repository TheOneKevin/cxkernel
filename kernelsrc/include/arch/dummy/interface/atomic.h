/*
 * File:   atomic.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 2018-03-28T14:32:10-04:00
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2018-03-28T14:33:53-04:00
*/

#pragma once

#include "common.h"

typedef struct { volatile int counter; } atomic_t;
#define ATOMIC_INIT(i) (i)
