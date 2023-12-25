# Include DOCTEST framework
include(FetchContent)
FetchContent_Declare(
        doctest
        GIT_REPOSITORY  https://github.com/doctest/doctest
        GIT_TAG         v2.4.11
)
FetchContent_MakeAvailable(doctest)
# Expose required variable (DOCTEST_INCLUDE_DIR) to parent scope
set(DOCTEST_INCLUDE_DIR ${doctest_SOURCE_DIR}/doctest CACHE INTERNAL "Path to include folder for doctest")
# Include scripts/cmake/doctest.cmake from doctest
include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)

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
        -lgcc -nostdlib -ffreestanding -g -gdwarf-4
)
set(STANDARD_CXX_FLAGS
        -Wall -Wextra -Werror -g -gdwarf-4 -Og
        -fno-rtti -fno-exceptions -nostdinc++
        -fno-omit-frame-pointer -fno-stack-protector
        -ffreestanding -mno-red-zone -Wthread-safety
)

# Set up coverage flags for clang
set(CLANG_COVERAGE_OPTIONS -fprofile-instr-generate -fcoverage-mapping)

# Add new test function
function(add_llvm_coverage_test TEST_NAME)
        # Create executable
        add_executable(${TEST_NAME} ${ARGN})
        # Link against kernel libraries
        target_include_directories(
                ${TEST_NAME}
                PRIVATE
                ${CMAKE_CURRENT_SOURCE_DIR}/lib
                ${CMAKE_CURRENT_SOURCE_DIR}/core/include
                ${CMAKE_CURRENT_SOURCE_DIR}/arch
                ${CMAKE_CURRENT_SOURCE_DIR}
                PUBLIC
                ${DOCTEST_INCLUDE_DIR}
        )
        # Add coverage and debug flags
        target_compile_options(${TEST_NAME} PRIVATE -g ${CLANG_COVERAGE_OPTIONS})
        target_link_options(${TEST_NAME} PRIVATE ${CLANG_COVERAGE_OPTIONS})
        # Add test to CTest using the doctest test runner
        doctest_discover_tests(${TEST_NAME})
        # FIXME: Coverage is not working
        # PROPERTIES ENVIRONMENT "LLVM_PROFILE_FILE=${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME}.profraw"
endfunction()

# Enable testing!
enable_testing()
