/*
 * @file   start.s
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Monday, October 8th 2018, 4:44:15 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-10-08T16:47:41-04:00
 */

_start:
    call _init
    call kernel_main
    call _fini
0:  jmp 0b
