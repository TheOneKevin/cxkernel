/**
 * Copyright (c) 2020 The cxkernel Authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 * 
 * @file   alloc.cc
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on March 02 2020, 8:42 PM
 */

#include "core/memory.h"
#pragma GCC diagnostic ignored "-Wunused-parameter"

void* operator new(size_t sz) noexcept
{
	return kmem::kmalloc(sz);
}

void* operator new[](size_t sz) noexcept
{
	return kmem::kmalloc(sz);
}

void operator delete(void* p) noexcept
{
	kmem::kfree(p);
}

void operator delete[](void* p) noexcept
{
	kmem::kfree(p);
}

void operator delete(void* p, size_t sz) noexcept
{
	kmem::kfree(p);
}

void operator delete[](void* p, size_t sz) noexcept
{
	kmem::kfree(p);
}