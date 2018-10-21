/**
 * @file   platform.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 *
 * @date Created on Saturday, October 13th 2018, 6:24:19 pm
 *
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-10-19T16:10:21-04:00
 */

#pragma once
#include "common.h"
#include "console.h"

#ifdef __cplusplus

namespace Platform
{
    Console& GetConsole(void);
    void     Init(void);
}

#endif
