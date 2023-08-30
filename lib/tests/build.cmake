# Get all source files
file(GLOB SOURCES "${CMAKE_CURRENT_LIST_DIR}/*.cc")
list(
        APPEND
        SOURCES
        # Add .cc sources here
)

# Define library_tests target
add_executable(library_tests ${SOURCES})
target_include_directories(
        library_tests
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/lib
        ${CMAKE_CURRENT_SOURCE_DIR}/core/include
        ${CMAKE_CURRENT_SOURCE_DIR}
        PUBLIC
        ${DOCTEST_INCLUDE_DIR}
)

add_llvm_coverage_test(library_tests)

# TODO: Run these 3 commands after...
# llvm-profdata merge -sparse library_tests.profraw -o library_tests.profdata
# llvm-cov show -output-dir=coverage/report -format=html -instr-profile=library_tests.profdata -object=library_tests
# llvm-cov export -format=lcov -instr-profile=library_tests.profdata -object=library_tests > coverage/lcov.info
# --ignore-filename-regex="build|lib/tests/.*"
