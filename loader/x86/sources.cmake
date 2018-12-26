# @file   sources.cmake
# @author Kevin Dai \<kevindai02@outlook.com\>
#
# @date Created on Sunday, October 28th 2018, 3:00:56 pm
#
# @date Last modified by:   Kevin Dai
# @date Last modified time: 2018-11-13T16:44:47-05:00

list(APPEND LSOURCES2 "${CMAKE_CURRENT_SOURCE_DIR}/lib/libc/printk.c")
list(APPEND LSOURCES2 "${CMAKE_CURRENT_SOURCE_DIR}/lib/libc/vsprintf.c")
list(APPEND LSOURCES2 "${CMAKE_CURRENT_SOURCE_DIR}/lib/libc/string.c")
list(APPEND LSOURCES2 "${CMAKE_CURRENT_SOURCE_DIR}/lib/libc/panic_assert.c")
list(APPEND LSOURCES1 "${CMAKE_CURRENT_SOURCE_DIR}/lib/elf/elf_parser.cc")
list(APPEND LSOURCES1 "${CMAKE_CURRENT_SOURCE_DIR}/arch/x86/cpu.cc")
