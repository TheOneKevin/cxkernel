add_custom_target(
        kernel.iso ALL
        COMMAND
                ${CMAKE_CURRENT_SOURCE_DIR}/scripts/make_grub_image.sh
                ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_LIST_DIR}
        DEPENDS cxkernel
        DEPENDS cxloader
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
