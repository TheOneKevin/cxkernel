# Copyright (c) 2019 The cxkernel Authors. All rights reserved.
# Use of this source code is governed by a MIT-style
# license that can be found in the LICENSE file or at
# https://opensource.org/licenses/MIT
# 
# @file   unit.cmake
# @author Kevin Dai \<kevindai02@outlook.com\>
# @date   Created on December 28 2019, 11:18 AM

include(CTest)
enable_testing()

set(CMAKE_C_COMPILER /usr/bin/gcc CACHE PATH "")
set(CMAKE_CXX_COMPILER /usr/bin/g++ CACHE PATH "")
set(IS_NOTWSL 0)
execute_process(COMMAND grep -q Microsoft /proc/version
                RESULT_VARIABLE IS_NOTWSL)

# set(CMAKE_BUILD_TYPE RELEASE)

include_directories("include")
include_directories("lib/ebl/include")
include_directories("lib/elf/include")
add_library(unity STATIC "tests/unity/unity.c")
target_include_directories(unity PUBLIC "tests/unity")

# One day I'll make this automatic

add_executable(uNDALC_drv "tests/unit_nodealloc.cc")
target_link_libraries(uNDALC_drv unity)

# Flags
if(${IS_NOTWSL} EQUAL 0)
    message("You're under WSL!")
    set_target_properties(uNDALC_drv PROPERTIES COMPILE_FLAGS "-Wno-cpp -fpermissive -Og")
else()
    set_target_properties(unity PROPERTIES COMPILE_FLAGS "-Wno-cpp -m32" LINK_FLAGS "-m32")
    set_target_properties(uNDALC_drv PROPERTIES COMPILE_FLAGS "-Wno-cpp -m32 -O1" LINK_FLAGS "-m32")
endif()

# Add tests

add_test(NAME "NDALC_Edge_Cases" COMMAND uNDALC_drv 0)
add_test(NAME "NDALC_Alloc_Free" COMMAND uNDALC_drv 1)
add_test(NAME "NDALC_Alloc_Error" COMMAND uNDALC_drv 2)
add_test(NAME "NDALC_Alloc_Contig" COMMAND uNDALC_drv 4)
