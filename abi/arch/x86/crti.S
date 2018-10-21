/*
 * @file   crti.s
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Monday, October 8th 2018, 2:01:45 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-10-08T14:02:36-04:00
 */

.section .init
.global _init
.type _init, @function
_init:
    push %ebp
    movl %esp, %ebp

.section .fini
.global _fini
.type _fini @function
_fini:
    push %ebp
    movl %esp, %ebp
