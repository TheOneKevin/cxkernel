cmake_minimum_required(VERSION 3.5)
project(osdev)
include(build/utils.cmake)

# All configure options
option(TARGET "Target platform" "x86")
option(BITS "Target bits (64 or 32)" "x86_32")
option(TOOLCHAIN_PATH "Path of the cross compiler toolchain" "~/barebones-toolchain/cross/x86_64/bin/")
option(PRXDIR "This is the absolute path to the project folder (optional)" "")
option(CUSTOM_FILENAME "Use the custome __FILENAME__ instead of __FILE__" OFF)
option(LOG_ENABLE "Enable logging to serial" ON)
option(LOG_ALL_OUTPUT "Redirect all stdout to screen and serial" ON)
option(DEBUG "Enable kernel debugging" OFF)

# CMake printing options
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Default flags that shouldn't really change
set(CMAKE_C_FLAGS   "-Wall -Wextra -ffreestanding -D__KERNEL__ -std=gnu99 ")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -ffreestanding -D__KERNEL__ -std=c++14 ")
set(CMAKE_CXX_LINK_FLAGS "-lgcc -nostdlib -L${PROJECT_SOURCE_DIR}/build/obj -lopenlibm ")

# Target platform selection
if(${TARGET} MATCHES ^i*86*$|^x86_32$|IA32|x86)
    message("Building for x86_32")
    # Setup variables
    set(TOOLCHAIN_PREFIX i686-elf-)
    set(BITS x86_32)
    # Set up assembler
    enable_language(ASM-ATT)
    set(CMAKE_ASM_NASM_FLAGS -g)
    set(CMAKE_ASM_NASM_OBJECT_FORMAT elf)
    enable_language(ASM_NASM)
    set(CMAKE_ASM-ATT_SOURCE_FILE_EXTENSIONS s;S;i)
    set(CMAKE_ASM_NASM_SOURCE_FILE_EXTENSIONS nasm;asm)
    # Define flags
    string(APPEND CMAKE_CXX_FLAGS "-fno-rtti -fno-exceptions -nostdinc++ -fno-omit-frame-pointer ")
    string(APPEND CMAKE_C_FLAGS "-fno-omit-frame-pointer ")
    if(${DEBUG})
        string(APPEND CMAKE_CXX_FLAGS "-Og -g -g3 ")
        string(APPEND CMAKE_CXX_FLAGS "-Og -g -g3 ")
        string(APPEND CMAKE_CXX_LINK_FLAGS "-g ")
    else()
        string(APPEND CMAKE_CXX_FLAGS "-O3 ")
        string(APPEND CMAKE_CXX_FLAGS "-O3 ")
    endif()
    # Set up toolchain
    include(build/elf-toolchain.cmake)
elseif(${TARGET} MATCHES ^x86_64$|^AMD64$|^IA64$)
    message("Building for IA64 (AMD64)")
    set(BITS x86_64)
else()
    message("No target specified. Building for x86_32")
endif()

# Set home directory
if(NOT HOME)
    if(WIN32)
        string(REPLACE "\\" "/" HOME "$ENV{USERPROFILE}")
    else()
        set(HOME $ENV{HOME})
    endif()
endif()

# Generate header
configure_file(build/config.h.in ../config.h)

# Include headers
include_directories(build)
include_directories(include)
include_directories(lib/elf/include)
include_directories(lib/libc/include)
include_directories(lib/uclibcxx/include)
include_directories(lib/ebl/include)
include_directories(lib/openlibm/include)
include_directories(lib/openlibm/src)

# Set output directory
set(EXECUTABLE_OUTPUT_PATH ../bin)

# Build kernel
set(CXKRNL_NPRUNE /core/|/lib/|/platform/|/arch/${TARGET})
set(CXKRNL_PPRUNE /loader/|/lib/openlibm/)
file(GLOB_RECURSE SOURCES1 *.cc *.cpp)
file(GLOB_RECURSE SOURCES2 *.c)
file(GLOB_RECURSE SOURCES3 *.S)
file(GLOB_RECURSE SOURCES4 *.asm)
nprune(SOURCES1 ${CXKRNL_NPRUNE})
nprune(SOURCES2 ${CXKRNL_NPRUNE})
nprune(SOURCES3 ${CXKRNL_NPRUNE})
nprune(SOURCES4 ${CXKRNL_NPRUNE})
pprune(SOURCES1 ${CXKRNL_PPRUNE})
pprune(SOURCES2 ${CXKRNL_PPRUNE})
pprune(SOURCES3 ${CXKRNL_PPRUNE})
pprune(SOURCES4 ${CXKRNL_PPRUNE})
set(SOURCES ${SOURCES1} ${SOURCES2} ${SOURCES3} ${SOURCES4})

# Preprocess assembly
foreach(_file ${SOURCES3})
    LIST(REMOVE_ITEM SOURCES3 ${_file})
    string(REPLACE ".S" "" _nfile "${_file}") # Capital .S
    add_custom_target(
            PREPROCESS_${_nfile}
            COMMAND ${TOOLCHAIN_PREFIX}gcc -nostdlib -ffreestanding -I$<JOIN:$<TARGET_PROPERTY:INCLUDE_DIRECTORIES>, -I> -x assembler-with-cpp -E ${_file} -o ${_nfile}.i
    )
    LIST(APPEND SOURCE3 ${_nfile})
    add_dependencies(cxkrnl PREPROCESS_${_nfile})
endforeach()

# Build binary
add_executable(cxkrnl ${SOURCES})
lprune_target(cxkrnl "${PRXDIR}")
set_target_properties(cxkrnl PROPERTIES LINK_FLAGS "-T ${CMAKE_CURRENT_SOURCE_DIR}/build/obj/arch/${BITS}_linker.ld")
set_target_properties(cxkrnl PROPERTIES SUFFIX ".sys")

# Build loader
set(CXLODR_NPRUNE /loader/${TARGET})
file(GLOB_RECURSE LSOURCES1 *.cc *.cpp)
file(GLOB_RECURSE LSOURCES2 *.c)
file(GLOB_RECURSE LSOURCES3 *.S)
file(GLOB_RECURSE LSOURCES4 *.asm)
nprune(LSOURCES1 ${CXLODR_NPRUNE})
nprune(LSOURCES2 ${CXLODR_NPRUNE})
nprune(LSOURCES3 ${CXLODR_NPRUNE})
nprune(LSOURCES4 ${CXLODR_NPRUNE})
include(loader/${TARGET}/sources.cmake)
set(LSOURCES ${LSOURCES1} ${LSOURCES2} ${LSOURCES3} ${LSOURCES4})

# Build binary
add_executable(cxlodr ${LSOURCES})
lprune_target(cxlodr "${PRXDIR}")
target_include_directories(cxlodr PUBLIC loader/${TARGET}/include)
set_target_properties(cxlodr PROPERTIES LINK_FLAGS "-T ${CMAKE_CURRENT_SOURCE_DIR}/loader/obj/arch/${TARGET}_linker.ld")
set_target_properties(cxlodr PROPERTIES SUFFIX ".bin")

# Build custom libraries
add_custom_target(
        libopenlibm
        DEPENDS build/obj/libopenlibm.a
        VERBATIM
)
add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/build/obj/libopenlibm.a
        COMMAND make clean
        COMMAND make libopenlibm.a TOOLPREFIX=${TOOLCHAIN_PREFIX} ARCH=i386
        COMMAND cp libopenlibm.a ../../build/obj/libopenlibm.a
        WORKING_DIRECTORY ../../lib/openlibm
        VERBATIM
)
add_dependencies(cxkrnl libopenlibm)
add_dependencies(cxlodr cxkrnl)

# Generate image
add_custom_command(
        OUTPUT  LiquiDOS.iso
        DEPENDS cxlodr
        DEPENDS cxkrnl
        COMMAND . ./scripts/${BITS}_buildimg.sh
        WORKING_DIRECTORY ../
        VERBATIM
)

# Build all
add_custom_target(
        iso ALL
        DEPENDS LiquiDOS.iso
        COMMAND
        VERBATIM
)

# DONE!