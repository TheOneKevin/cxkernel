include(CTest)
enable_testing()

include_directories("include")
include_directories("lib/ebl/include")
include_directories("lib/elf/include")
add_library(unity STATIC "tests/unity/unity.c")
set_target_properties(unity PROPERTIES COMPILE_FLAGS "-Wno-cpp -m32" LINK_FLAGS "-m32")
target_include_directories(unity PUBLIC "tests/unity")

add_executable(UnitTest_NodeAllocator_App "tests/native.cc")
set_target_properties(UnitTest_NodeAllocator_App PROPERTIES COMPILE_FLAGS "-Wno-cpp -m32" LINK_FLAGS "-m32")
target_link_libraries(UnitTest_NodeAllocator_App unity)
add_test(UnitTest_NodeAllocator UnitTest_NodeAllocator_App)
