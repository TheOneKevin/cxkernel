set(SOURCES "")

# Add all sources
file(GLOB SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/core/assert.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/lib/ebl/*.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/lib/elf/*.cc"
        "${CMAKE_CURRENT_LIST_DIR}/*.cc"
        "${CMAKE_CURRENT_LIST_DIR}/*.asm"
)

# Manually set the language + flags for the assembly file
set_source_files_properties(
        "${CMAKE_CURRENT_LIST_DIR}/start.asm"
        PROPERTIES
        LANGUAGE ASM_NASM
        COMPILE_FLAGS -felf
)

# Update target sources
target_sources(cxloader PRIVATE ${SOURCES})

# Update target include paths
target_include_directories(
        cxloader PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/lib"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/"
        "${CMAKE_CURRENT_LIST_DIR}/"
)

# Set compile and linker flags
target_link_options(
        cxloader PRIVATE
        ${STANDARD_LINKER_FLAGS}
        -T ${CMAKE_CURRENT_LIST_DIR}/linker.ld
        LINKER:--apply-dynamic-relocs
        LINKER:-z,max-page-size=0x1000
        -mabi=sysv -m32
)
target_compile_options(
        cxloader PRIVATE
        $<$<COMPILE_LANGUAGE:CXX>:${STANDARD_CXX_FLAGS} -mabi=sysv -m32>
)

# Update target properties
set_target_properties(cxloader PROPERTIES SUFFIX ".bin")
