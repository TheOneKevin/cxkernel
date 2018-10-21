/*
 * @file   crtn.s
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Monday, October 8th 2018, 2:02:45 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-10-08T14:03:10-04:00
 */

.section .init
    popl %ebp
    ret

.section .fini
    popl %ebp
    ret
