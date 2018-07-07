/*
 * File:   kprintf-test.c
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * @date Created on 01-Aug-2017 04:14:22 PM
 *
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2017-08-26T19:28:25-04:00
*/

#include "lib/printk.h"
#include "lib/string.h"
#include "assert.h"

static const char* test_msg[9] =
{
    "[1]: Hello, world!\n",
    "[2]: Hello, world! deadbeef CAFEBABE 10 10 c % 100 -10 \n",
    "[3]: 0X00DEAD 0x0000beef  010 0010\n",
    "[4]: -9223372036854775807 0xFEFFFFFFBFFFFFFF 0xBEEF 1\n",
    "[5]:  -1024     string          c\n",
    "[6]:   1024    -123456 0XFEEDFACE\n",
    "[7]: AFC0FFEE DECAFC0FFEE\n",
    "[8]: poop is fun         fun is poop\n",
    "[9]: -1024 1024 BEEF cafe 10 +1024  1024 0XBEEF 0xcafe 010 10\n\n"
};

void __test_kprintf__()
{
    char buf[80];
    long long myLong = -9223372036854775807L;
    memset((void *) buf, 0, 80);

    sprintf(buf, "[1]: Hello, world!\n");
    ASSERT(strcmp(buf, test_msg[0]) == 0, "printk test case [1] failed");
    memset((void *) buf, 0, 80);

    sprintf(buf, "[2]: %s %x %X %u %o %c %% %d %i \n", "Hello, world!", 0xDEADBEEF, 0xCAFEBABE, 10, 8, 'c', 'd', -10, 0);
    ASSERT(strcmp(buf, test_msg[1]) == 0, "printk test case [2] failed");
    memset((void *) buf, 0, 80);

    sprintf(buf, "[3]: %0#8X %#010x %#4o %0*u\n", 0xDEAD, 0xBEEF, 8, 4, 10);
    ASSERT(strcmp(buf, test_msg[2]) == 0, "printk test case [3] failed");
    memset((void *) buf, 0, 80);

    sprintf(buf, "[4]: %ld 0x%lX 0x%hX %hi\n", myLong, 0xFEFFFFFFBFFFFFFFL, 0xDEADBEEF, myLong);
    ASSERT(strcmp(buf, test_msg[3]) == 0, "printk test case [4] failed");
    memset((void *) buf, 0, 80);

    sprintf(buf, "[5]: %+ 6d %10s %10c\n", -1024, "string", 'c');
    ASSERT(strcmp(buf, test_msg[4]) == 0, "printk test case [5] failed");
    memset((void *) buf, 0, 80);

    sprintf(buf, "[6]: % 6d %10i %#010X\n", 1024, -123456, 0xFEEDFACE);
    ASSERT(strcmp(buf, test_msg[5]) == 0, "printk test case [6] failed");
    memset((void *) buf, 0, 80);

    sprintf(buf, "[7]: %X %lX\n", (int) 0xDECAFC0FFEEL, 0xDECAFC0FFEEL);
    ASSERT(strcmp(buf, test_msg[6]) == 0, "printk test case [7] failed");
    memset((void *) buf, 0, 80);

    sprintf(buf, "[8]: %-15s %15s\n", "poop is fun", "fun is poop");
    ASSERT(strcmp(buf, test_msg[7]) == 0, "printk test case [8] failed");
    memset((void *) buf, 0, 80);

    sprintf(buf, "[9]: %d %u %X %x %o %+d % d %#X %#x %#o %#u\n\n", -1024, 1024, 0xBEEF, 0xCAFE, 8, 1024, 1024, 0xBEEF, 0xCAFE, 8, 10);
    ASSERT(strcmp(buf, test_msg[8]) == 0, "printk test case [9] failed");
    memset((void *) buf, 0, 80);
}
