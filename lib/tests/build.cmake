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
