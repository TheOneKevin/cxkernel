// ref: https://www.gnu.org/software/grub/manual/multiboot2/multiboot.html

#include <stddef.h>
#include "loader.h"

using multiboot_iter_t = multiboot_tag_list::iterator;
using mmap_iter_t = multiboot_mmap_list::iterator;

//===----------------------------------------------------------------------===//
// Multiboot info tag list iterator

struct multiboot_tag* multiboot_iter_t::operator *() {
    while(true) {
        if(tag->type == type) {
            return tag;
        }

        if(tag->type == MULTIBOOT_TAG_TYPE_END) {
            tag = nullptr;
            return tag;
        }

        this->operator++();
    }
}

multiboot_iter_t& multiboot_iter_t::operator++() {
    tag = (struct multiboot_tag*)
        ((multiboot_uint8_t*) tag + ((tag->size + 0b111) & ~0b111));
    if(tag->type == MULTIBOOT_TAG_TYPE_END) {
        tag = nullptr;
    }
    return *this;
}

//===----------------------------------------------------------------------===//
// Multiboot memory map iterator

mmap_iter_t& mmap_iter_t::operator++() {
    auto stride = mmap->entry_size;
    entry = (multiboot_memory_map_t*)((multiboot_uint8_t*) entry + stride);
    if((multiboot_uint8_t*) entry >= (multiboot_uint8_t*) mmap + mmap->size) {
        entry = nullptr;
    }
    return *this;
}
