cmake_minimum_required(VERSION 3.5)
project(osdev)
include(build/utils.cmake)

# Default flags that shouldn't really change
set(CMAKE_C_FLAGS   "-Wall -Wextra -ffreestanding -D__KERNEL__ -std=gnu99 ")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -ffreestanding -D__KERNEL__ -std=c++14 ")
set(CMAKE_CXX_LINK_FLAGS "-lgcc -nostdlib")

# Target selection
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
    string(APPEND CMAKE_CXX_FLAGS "-fno-rtti -fno-exceptions -nostdinc++ -fno-omit-frame-pointer")
    string(APPEND CMAKE_C_FLAGS "-fno-omit-frame-pointer")
    # Set up toolchain
    include(build/elf-toolchain.cmake)
elseif(${TARGET} MATCHES ^x86_64$|^AMD64$|^IA64$)
    message("Building for IA64 (AMD64)")
    set(BITS x86_64)
else()
    message("No target specified. Building for x86_32")
endif()

if(NOT HOME)
    if(WIN32)
        string(REPLACE "\\" "/" HOME "$ENV{USERPROFILE}")
    else()
        set(HOME $ENV{HOME})
    endif()
endif()

include_directories(build)
include_directories(include)
include_directories(lib/elf/include)
include_directories(lib/libc/include)
include_directories(lib/libc++/include)
include_directories(lib/ebl/include)
set(EXECUTABLE_OUTPUT_PATH ../bin)

# Build kernel
file(GLOB_RECURSE SOURCES1 *.cc *.cpp)
file(GLOB_RECURSE SOURCES2 *.c)
file(GLOB_RECURSE SOURCES3 *.S)
file(GLOB_RECURSE SOURCES4 *.asm)
nprune(SOURCES1 /core/|/lib/|/platform/|/arch/${TARGET})
nprune(SOURCES2 /core/|/lib/|/platform/|/arch/${TARGET})
nprune(SOURCES3 /core/|/lib/|/platform/|/arch/${TARGET})
nprune(SOURCES4 /core/|/lib/|/platform/|/arch/${TARGET})
pprune(SOURCES1 /loader/)
pprune(SOURCES2 /loader/)
pprune(SOURCES3 /loader/)
pprune(SOURCES4 /loader/)
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
file(GLOB_RECURSE LSOURCES1 *.cc *.cpp)
file(GLOB_RECURSE LSOURCES2 *.c)
file(GLOB_RECURSE LSOURCES3 *.S)
file(GLOB_RECURSE LSOURCES4 *.asm)
nprune(LSOURCES1 /loader/${TARGET})
nprune(LSOURCES2 /loader/${TARGET})
nprune(LSOURCES3 /loader/${TARGET})
nprune(LSOURCES4 /loader/${TARGET})
include(loader/${TARGET}/sources.cmake)
set(LSOURCES ${LSOURCES1} ${LSOURCES2} ${LSOURCES3} ${LSOURCES4})

add_executable(cxlodr ${LSOURCES})
lprune_target(cxlodr "${PRXDIR}")
target_include_directories(cxlodr PUBLIC loader/${TARGET}/include)
set_target_properties(cxlodr PROPERTIES LINK_FLAGS "-T ${CMAKE_CURRENT_SOURCE_DIR}/loader/obj/arch/${TARGET}_linker.ld")
set_target_properties(cxlodr PROPERTIES SUFFIX ".bin")

# Generate image
add_custom_command(
        OUTPUT  LiquiDOS.iso
        DEPENDS cxlodr
        DEPENDS cxkrnl
        COMMAND . ./scripts/${BITS}_buildimg.sh
        WORKING_DIRECTORY ../
        VERBATIM
)
add_custom_target(
        iso ALL
        DEPENDS LiquiDOS.iso
        COMMAND
        VERBATIM
)

# DONE!