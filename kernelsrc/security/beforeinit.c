#include "multiboot.h"
#include "rand/rand.h"
#include "system/time.h"
#include <stdint.h>

extern multiboot_info_t* mbt;
uint32_t __stack_chk_guard;
void before_main(multiboot_info_t* multi)
{
    asm volatile("cli");
    mbt = multi; //Store the multiboot header in case we accidently corrupt it

    read_rtc();

    randctx ctx;
    ctx.randa = ctx.randb = ctx.randc = (uint32_t) 0;

    uint64_t epoch = getEpoch();
    uint32_t hi = (epoch >> 32) + 1; //+1 to make it non-zero
    uint32_t lo = (epoch & 0x00000000FFFFFFFF) + 1;

    init_rand(hi + lo);
    ctx.randrsl [0] = rand_cmwc();

    // Generate the seed based on the time, and reseed the generator based on previous output
    // Is this high enough entropy for the seed?
    for(int i = 1; i < RANDSIZ; ++i)
    {
        if(i % 8 == 0) //Change seed every 8 generations
        {
            init_rand((uint32_t) (lo * rand_cmwc() * ctx.randrsl [i - 1]));
            ctx.randrsl [i] = (uint32_t) rand_cmwc();
        }
        else
        {
            init_rand((uint32_t) (hi * rand_cmwc()  * ctx.randrsl [i - 1]));
            ctx.randrsl [i] = (uint32_t) rand_cmwc();
        }
    }

    randinit (&ctx, TRUE); // Intialize the ISAAC CS-PRNG with the seed that we have generated
    __stack_chk_guard = rand(&ctx);
}
