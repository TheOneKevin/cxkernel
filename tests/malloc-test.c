/*
 * Filename: malloc-test.c
 * Author:   Kevin Dai
 * Email:    kevindai02@outlook.com
 *
 * Created on 11-Aug-2017 04:11:43 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 12-Aug-2017 02:08:35 PM
*/

#include "sys/malloc.h"
#include "lib/assert.h"

extern void __malloc_check();

void __test_malloc___()
{
    size_t mem_old1 = malloc_mem_used;
    size_t mem_old2 = malloc_mem_free;
    char* c = (char *) malloc(1024);
    free(c);
    ASSERT(mem_old1 == malloc_mem_used, "Malloc test 1 failed.");
    ASSERT(mem_old2 == malloc_mem_free, "Malloc test 2 failed.");
    free(c);
    ASSERT(mem_old1 == malloc_mem_used, "Malloc test 3 failed.");
    ASSERT(mem_old2 == malloc_mem_free, "Malloc test 4 failed.");
    __malloc_check();
}
