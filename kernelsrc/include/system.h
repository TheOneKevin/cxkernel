/**
 * @file   system.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Sunday, November 26th 2017, 9:42:00 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-07-22T17:19:05-04:00
 */

#pragma once

#include "common.h"
#include "tasking/task.h"
#include "arch/arch_interface.h"

process_t* g_current_process = NULL;
