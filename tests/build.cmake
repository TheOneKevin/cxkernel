file(GLOB EBL_TEST_SOURCES "${CMAKE_CURRENT_LIST_DIR}/ebl_*.cc")
add_llvm_coverage_test(
        test_ebl
        ${EBL_TEST_SOURCES}
        "${CMAKE_CURRENT_LIST_DIR}/assert.cc"
)

add_llvm_coverage_test(
        test_kernel_vmm
        "${CMAKE_CURRENT_LIST_DIR}/assert.cc"
        "${CMAKE_CURRENT_LIST_DIR}/kernel_vmm.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/vmregion.cc"
)

# TODO: Run these 3 commands after "make test"
# llvm-profdata merge -sparse library_tests.profraw -o library_tests.profdata
# llvm-cov show -output-dir=coverage/report -format=html -instr-profile=library_tests.profdata -object=library_tests
# llvm-cov export -format=lcov -instr-profile=library_tests.profdata -object=library_tests > coverage/lcov.info
# --ignore-filename-regex="build|lib/tests/.*"
