# Define library_tests target
file(GLOB EBL_TEST_SOURCES "${CMAKE_CURRENT_LIST_DIR}/ebl_*.cc")
add_executable(library_tests ${EBL_TEST_SOURCES})
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

# Define kernel_tests target
add_executable(
        kernel_tests
        "${CMAKE_CURRENT_LIST_DIR}/assert.cc"
        "${CMAKE_CURRENT_LIST_DIR}/kernel_vmm.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/vmregion.cc"
)
target_include_directories(
        kernel_tests
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/lib
        ${CMAKE_CURRENT_SOURCE_DIR}/core/include
        ${CMAKE_CURRENT_SOURCE_DIR}/arch
        ${CMAKE_CURRENT_SOURCE_DIR}
        PUBLIC
        ${DOCTEST_INCLUDE_DIR}
)
add_llvm_coverage_test(kernel_tests)

# TODO: Run these 3 commands after "make test"
# llvm-profdata merge -sparse library_tests.profraw -o library_tests.profdata
# llvm-cov show -output-dir=coverage/report -format=html -instr-profile=library_tests.profdata -object=library_tests
# llvm-cov export -format=lcov -instr-profile=library_tests.profdata -object=library_tests > coverage/lcov.info
# --ignore-filename-regex="build|lib/tests/.*"
