/**
 * @file   platform.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 *
 * @date Created on Saturday, October 13th 2018, 6:24:19 pm
 *
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-11-13T20:14:52-05:00
 */

#pragma once
#include "common.h"
#include "console.h"

namespace platform  {
    Console& get_console(void);
    void early_init(void);
    void init(void);
}
