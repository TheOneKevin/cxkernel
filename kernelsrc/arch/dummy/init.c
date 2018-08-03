/*
 * Filename: init.c
 * Author:   Kevin Dai
 * Email:    kevindai02@outlook.com
 *
 * @date Created on 2018-03-28T14:35:15-04:00
 *
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-03-28T14:40:18-04:00
*/

#define __MODULE__ "INIT "

#include "arch/arch_interface.h"

console_t dummy_cons =
{
    .putc  = NULL,
    .clear = NULL,
    .getc  = NULL
};

console_t* arch_get_console(void)
{
    return &dummy_cons;
}

void arch_early_init(uint32_t magic, void* ptr)
{

}

void arch_init(void)
{

}

void arch_late_init(void)
{

}

void arch_pmeminit(void)
{

}
