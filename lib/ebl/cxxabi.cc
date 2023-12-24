// Required functions for C++ ABI to link against
// Most of these are just dummy functions that do nothing

unsigned __atexit_funcs_count = 0;
void *__dso_handle = 0;

namespace __cxxabiv1 {

   extern "C" int __cxa_atexit(void (*f)(void *), void *objptr, void *dso) {
      (void)f;
      (void)objptr;
      (void)dso;
      return 0;
   }

   extern "C" void __cxa_finalize(void *f) { (void)f; }

   void __cxa_pure_virtual() {
      // Do nothing
      for(;;)
         ;
   }

   // The ABI requires a 64-bit type.
   __extension__ typedef int __guard __attribute__((mode(__DI__)));

   extern "C" int __cxa_guard_acquire(__guard *);
   extern "C" void __cxa_guard_release(__guard *);
   extern "C" void __cxa_guard_abort(__guard *);

   extern "C" int __cxa_guard_acquire(__guard *g) { return !*(char *)(g); }

   extern "C" void __cxa_guard_release(__guard *g) { *(char *)g = 1; }

   extern "C" void __cxa_guard_abort(__guard *g) { (void)g; }

} // namespace __cxxabiv1
