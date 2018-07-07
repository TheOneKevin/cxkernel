/*
 * File:   inittests.c
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * @date Created on 01-Aug-2017 04:14:31 PM
 *
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 11-Aug-2017 04:11:51 PM
*/

extern void __test_malloc___();
extern void __test_kprintf__();

void __initKernelTests__()
{
    __test_malloc___();
    __test_kprintf__();
}
