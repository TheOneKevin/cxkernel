/**
 * Copyright (c) 2019 The cxkernel authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 *
 * @file   kernel.cc
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on October 7 2018, 4:12 PM
 */

#include <stdio.h>
#include <icxxabi.h>

#include "platform.h"
#include "core/vm.h"
#include "arch/interface.h"

// Random ctor shit
using ctor_func = void (*)();
extern ctor_func _ctors_start;
extern ctor_func _ctors_end;
__NOINLINE __NO_OPTIMIZE void dummy_ctor(void) { } EXPORT_CTOR(dummy_ctor);

extern "C" void kernel_main(loader_t args)
{
    // Execute ctors, these really only initializes printf
    for(ctor_func* func_arr = &_ctors_start; func_arr != &_ctors_end; func_arr++) (*func_arr)();
    platform::get_console().clear();
    arch::early_init(args);
    platform::early_init();
    platform::init();
    // Init memory functions

    // Do some final arch inits
    arch::init();
    //Execute start module
    for(;;);
}
