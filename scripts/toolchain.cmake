# Set up assembler
# ref: Github Kitware/CMake/blob/master/Modules/CMakeASM_NASMInformation.cmake
set(CMAKE_ASM_NASM_FLAGS -g)
set(CMAKE_ASM_NASM_COMPILE_OBJECT "<CMAKE_ASM_NASM_COMPILER> <FLAGS> -o <OBJECT> <SOURCE>")
enable_language(ASM_NASM)
enable_language(ASM-ATT)

# Set up linker
set(CMAKE_LINKER ${CMAKE_C_COMPILER})
set(CMAKE_CXX_LINK_EXECUTABLE
        "<CMAKE_LINKER> <LINK_FLAGS> <OBJECTS> -o <TARGET> <CMAKE_CXX_LINK_FLAGS>"
)
set(CONFIG_USE_PRIVATE_LIBGCC TRUE)

# Set up the compiler and linker
set(CMAKE_CXX_STANDARD 20)
set(STANDARD_LINKER_FLAGS
        -lgcc -nostdlib -ffreestanding -g
)
set(STANDARD_CXX_FLAGS
        -Wall -Wextra -g -Og
        -fno-rtti -fno-exceptions -nostdinc++
        -fno-omit-frame-pointer -fno-stack-protector
        -ffreestanding -mno-red-zone
)
