// Stack smash protector

#include <stdint.h>

#define RAND_STACK_CHK_GUARD 0xe2dee396

__attribute__((noreturn))
void __stack_chk_fail(void)
{
#if __STDC_HOSTED__
	abort();
#elif __is_myos_kernel
	panic("Stack smashing detected");
#endif
    for(;;); //Halt the kernel
}
