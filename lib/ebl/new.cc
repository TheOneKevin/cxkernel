#include <ebl/memory.h>
#include <stddef.h>
#ifndef LOADER
#include <core/slab.h>
#endif

#if __STDC_HOSTED__ == 0

void operator delete(void* ptr) noexcept {
#ifndef LOADER
   kmem::free(ptr);
#else
   (void)ptr;
   for(;;)
      ;
#endif
}

#endif
