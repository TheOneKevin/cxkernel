# Include DOCTEST framework
include(ExternalProject)
find_package(Git REQUIRED)
ExternalProject_Add(
    doctest
    PREFIX ${CMAKE_BINARY_DIR}/doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
    TIMEOUT 10
    UPDATE_COMMAND ${GIT_EXECUTABLE} pull
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
)

# Expose required variable (DOCTEST_INCLUDE_DIR) to parent scope
ExternalProject_Get_Property(doctest source_dir)
set(DOCTEST_INCLUDE_DIR ${source_dir}/doctest CACHE INTERNAL "Path to include folder for doctest")

# Set up assembler
# ref: Github Kitware/CMake/blob/master/Modules/CMakeASM_NASMInformation.cmake
set(CMAKE_ASM_NASM_FLAGS -g)
set(CMAKE_ASM_NASM_COMPILE_OBJECT "<CMAKE_ASM_NASM_COMPILER> <FLAGS> -o <OBJECT> <SOURCE>")
enable_language(ASM_NASM)
enable_language(ASM-ATT)

# Set up linker
set(CMAKE_LINKER ${CMAKE_CXX_COMPILER})
set(CMAKE_CXX_LINK_EXECUTABLE
        "<CMAKE_LINKER> <LINK_FLAGS> <OBJECTS> -o <TARGET> <CMAKE_CXX_LINK_FLAGS>"
)
set(CONFIG_USE_PRIVATE_LIBGCC TRUE)

# Set up the compiler and linker
set(STANDARD_LINKER_FLAGS
        -lgcc -nostdlib -ffreestanding -g
)
set(STANDARD_CXX_FLAGS
        -Wall -Wextra -Werror -g -Og
        -fno-rtti -fno-exceptions -nostdinc++
        -fno-omit-frame-pointer -fno-stack-protector
        -ffreestanding -mno-red-zone -Wthread-safety
)

# Set up coverage flags for clang
set(CLANG_COVERAGE_OPTIONS -fprofile-instr-generate -fcoverage-mapping)

# Add new test function
function(add_llvm_coverage_test TEST_NAME)
        target_compile_options(${TEST_NAME} PRIVATE ${CLANG_COVERAGE_OPTIONS})
        target_link_options(${TEST_NAME} PRIVATE ${CLANG_COVERAGE_OPTIONS})
        add_test(NAME ${TEST_NAME} COMMAND $<TARGET_FILE:${TEST_NAME}>)
        set_property(
                TEST ${TEST_NAME} PROPERTY ENVIRONMENT
                "LLVM_PROFILE_FILE=${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME}.profraw"
        )
endfunction()

# Enable testing!
enable_testing()
