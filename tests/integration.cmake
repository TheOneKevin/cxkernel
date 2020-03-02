# Copyright (c) 2019 The cxkernel Authors. All rights reserved.
# Use of this source code is governed by a MIT-style
# license that can be found in the LICENSE file or at
# https://opensource.org/licenses/MIT
# 
# @file   integration.cmake
# @author Kevin Dai \<kevindai02@outlook.com\>
# @date   Created on December 28 2019, 10:48 PM

list(APPEND SOURCES1 "${CMAKE_CURRENT_SOURCE_DIR}/tests/driver.cc")
list(APPEND SOURCES1 "${CMAKE_CURRENT_SOURCE_DIR}/tests/unity/unity_extern.cc")
list(APPEND SOURCES2 "${CMAKE_CURRENT_SOURCE_DIR}/tests/unity/unity.c")