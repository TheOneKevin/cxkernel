# @file   sources.mk
# @author Kevin Dai \<kevindai02@outlook.com\>
# 
# @date Created on Sunday, October 28th 2018, 3:00:56 pm
# 
# @date Last modified by:   Kevin Dai
# @date Last modified time: 2018-10-28T15:17:46-04:00

LIBINCLUDES := -I../lib/libc/include -I../include
SOURCES2 += ../lib/libc/printk.c
SOURCES2 += ../lib/libc/vsprintf.c
SOURCES2 += ../lib/libc/string.c
