// Stack smash protector

#include <stdint.h>

#pragma GCC diagnostic ignored "-Wimplicit-function-declaration"

uintptr_t __stack_chk_guard = 0xBADBEEF;

__attribute__((noreturn))
void __stack_chk_fail(void)
{
	panic("Stack smashing detected");
    for(;;); //Halt the kernel
}
