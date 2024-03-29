project(cxkernel)
cmake_minimum_required(VERSION 3.22)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set ... ???
set(TARGET_LOADER multiboot2)
set(TARGET_ARCH x86-64)
set(TARGET_PLATFORM pc)
# Set this depending on architecture
set(MCMODEL large)

# Setup toolchain
include(${CMAKE_CURRENT_SOURCE_DIR}/scripts/toolchain.cmake)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define macros
add_compile_definitions(ARCH_TARGET=x86-64)
add_compile_definitions(PLAT_TARGET=pc)
add_compile_definitions(ABI_ALIGN=8)

# Loader target
add_executable(cxloader "")
include(${CMAKE_CURRENT_SOURCE_DIR}/loader/${TARGET_ARCH}-${TARGET_LOADER}/build.cmake)

# Kernel target
file(GLOB LIB_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/lib/ebl/*.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/lib/elf/*.cc")
add_executable(cxkernel ${LIB_SOURCES})
set_target_properties(cxkernel PROPERTIES SUFFIX ".sys")
target_include_directories(
        cxkernel PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/lib"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/arch"
        "${CMAKE_CURRENT_SOURCE_DIR}/"
)
include(${CMAKE_CURRENT_SOURCE_DIR}/core/build.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/arch/${TARGET_ARCH}/build.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/platform/${TARGET_PLATFORM}/build.cmake)

# Final boot image target
include(${CMAKE_CURRENT_SOURCE_DIR}/scripts/grub.cmake)

# Library tests
include(${CMAKE_CURRENT_SOURCE_DIR}/tests/build.cmake)
