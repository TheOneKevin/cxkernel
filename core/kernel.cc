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
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <icxxabi.h>

#include "system.h"
#include "platform.h"
#include "core/memory.h"
#include "core/pmm_node.h"
#include "core/bootalloc.h"
#include "arch/interface.h"

// Random ctor shit
using ctor_func = void (*)();
extern ctor_func _ctors_start;
extern ctor_func _ctors_end;
__NOINLINE __NO_OPTIMIZE void dummy_ctor(void) { } EXPORT_CTOR(dummy_ctor);

// Some global variable stuff
static elf::Context ctx;
static loader_t __copy__loader = { 0, NULL, ctx, 0, NULL };
loader_t* g::loader;

extern "C" void kernel_main(loader_t args)
{
    // Execute ctors, these really only initializes printf
    for(ctor_func* func_arr = &_ctors_start; func_arr != &_ctors_end; func_arr++) (*func_arr)();
    platform::console.clear();
    
    // Copy loader structure over
    memcpy(&__copy__loader, &args, sizeof(loader_t));
    g::loader = &__copy__loader;
    
    // Transfer over loader bitmap allocator
    pmm::SetPhysicalAllocator(pmm::GetBootAllocator());
    pmm::GetPhysicalAllocator().AddArena(NULL, args.bitmap);
    
    // Start early init
    arch::early_init(args);
    platform::early_init();
    
    // Finish with final initialization
    arch::init();
    platform::init();
    
    // Switch allocator to linked list
    pmm::SetPhysicalAllocator(pmm::GetPmmNodeAllocator());
    platform::meminit();
    
    kmem_init();
    
    char* str = (char*) kmem_cache_alloc(strlen("Hello, World!") + 1);
    memset(str, 0, strlen("Hello, World!") + 1);
    str = "Hello, World!";
    printf("%s", str);
    fflush(STREAM_OUT);

    /*list_node_t pages;
    INIT_LLIST(&pages);
    pmm_alloc_pages(1, (uintptr_t) &pages);
    pmm_free((uintptr_t) &pages);*/

    //Execute start module
    for(;;);
}
