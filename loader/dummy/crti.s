/*
 * @file   crti.s
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Monday, October 8th 2018, 4:44:06 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-10-08T16:45:20-04:00
 */

.section .init
.global _init
.type _init, @function
_init:

.section .fini
.global _fini
.type _fini @function
_fini:
