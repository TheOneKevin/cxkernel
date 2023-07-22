set(SOURCES "")

# Add all sources
file(GLOB SOURCES
        "${CMAKE_CURRENT_LIST_DIR}/*.cc"
)

# Update target sources
target_sources(cxkernel PRIVATE ${SOURCES})
