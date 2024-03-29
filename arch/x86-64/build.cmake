set(SOURCES "")

# Add all sources
file(GLOB SOURCES
        "${CMAKE_CURRENT_LIST_DIR}/*.cc"
        "${CMAKE_CURRENT_LIST_DIR}/*.asm"
)

# Manually set the language + flags for the assembly file
set_source_files_properties(
        "${CMAKE_CURRENT_LIST_DIR}/entry.asm"
        PROPERTIES
        LANGUAGE ASM_NASM
        COMPILE_FLAGS "-felf64 -Fdwarf"
)

# Update target sources
target_sources(cxkernel PRIVATE ${SOURCES})

# Set compile and linker flags
target_link_options(
        cxkernel PRIVATE
        ${STANDARD_LINKER_FLAGS}
        LINKER:-T${CMAKE_CURRENT_LIST_DIR}/linker.ld
        LINKER:--apply-dynamic-relocs
        LINKER:-z,max-page-size=0x1000
        -no-pie -mabi=sysv -m64 -mcmodel=${MCMODEL}
)
target_compile_options(
        cxkernel PRIVATE
        $<$<COMPILE_LANGUAGE:CXX>:${STANDARD_CXX_FLAGS}
                -mcmodel=${MCMODEL} -fno-pie -m64>
)
