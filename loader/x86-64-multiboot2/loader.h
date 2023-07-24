#pragma once

#include "multiboot2.h"
#include <stdint.h>
#include <stddef.h>
#include "core/vm.h"

//===----------------------------------------------------------------------===//
// Functions

namespace arch {
    constexpr unsigned int page_size = 4096;
    constexpr vaddr_t page_align_down(vaddr_t addr) {
        return addr & ~(page_size - 1);
    }
    constexpr vaddr_t page_align_up(vaddr_t addr) {
        return (addr + page_size - 1) & ~(page_size - 1);
    }
};

void bootstrap_pmm(struct multiboot_tag_mmap *mmap);

//===----------------------------------------------------------------------===//
// Range class for memory ranges

struct range {
    range(uint64_t begin, uint64_t end) : begin{begin}, end{end} { }
    uint64_t begin;
    uint64_t end;
};

constexpr int num_reserved_ranges = 3;
extern range reserved_memory[num_reserved_ranges];

//===----------------------------------------------------------------------===//
// Iterator class for MBI tags

class multiboot_tag_list {
public:
    class iterator;
    multiboot_tag_list(unsigned long addr, multiboot_uint32_t type)
        : addr{addr}, type{type} { }
    iterator begin() const { return iterator{addr, type}; }
    iterator end() const { return iterator{}; }

public:
    class iterator {
        friend class multiboot_tag_list;
    public:
        iterator(unsigned long addr, multiboot_uint32_t type)
            : tag{(struct multiboot_tag*) (addr + 8)}, type{type} { }
        struct multiboot_tag* operator *();
        iterator& operator++();
        bool operator== (const iterator& a) const { return tag == a.tag; }
        bool operator!= (const iterator& a) const { return tag != a.tag; }
    private:
        iterator() : tag{nullptr}, type{0} {}
    private:
        struct multiboot_tag* tag;
        multiboot_uint32_t type;
    };

private:
    unsigned long addr;
    multiboot_uint32_t type;
};

//===----------------------------------------------------------------------===//
// Iterator class for MBI memory map

class multiboot_mmap_list {
public:
    class iterator;
    multiboot_mmap_list(struct multiboot_tag_mmap* mmap) : mmap{mmap} { }
    iterator begin() const { return iterator{mmap}; }
    iterator end() const { return iterator{}; }
public:
    class iterator {
        friend class multiboot_mmap_list;
    public:
        explicit iterator(struct multiboot_tag_mmap* mmap)
            : entry{mmap->entries}, mmap{mmap} { }
        multiboot_memory_map_t* operator *() const { return entry; }
        iterator& operator++();
        bool operator== (const iterator& a) const { return entry == a.entry; }
        bool operator!= (const iterator& a) const { return entry != a.entry; }
    private:
        iterator() : entry{nullptr}, mmap{nullptr} {}
    private:
        multiboot_memory_map_t* entry;
        struct multiboot_tag_mmap* mmap;
    };
private:
    struct multiboot_tag_mmap* mmap;
};
