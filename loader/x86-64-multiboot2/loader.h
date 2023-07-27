#pragma once
#define LOADER

#include "multiboot2.h"
#include <stdint.h>
#include <stddef.h>
#include "core/vm.h"
#include "arch/interface.h"

namespace ns = x86_64;
using page_node_t = core::pfndb_node_t;

struct range;
class multiboot_tag_list;
class multiboot_mmap_list;

extern paddr_t total_phys_pgs;
extern paddr_t pfndb_sz_bytes;
extern paddr_t pfndb_sz_pgs;
extern page_node_t* pfndb_arr;
extern core::pfndb_list_t pfndb_freelist;
extern core::pfndb_list_t pfndb_rsrvlist;

//===----------------------------------------------------------------------===//
// Functions

void bootstrap_pmm(const range (&res)[1], struct multiboot_tag_mmap *mmap);
uint64_t setup_paging();
void map_page(paddr_t phys, vaddr_t virt, ns::page_flags flags);
paddr_t pmm_alloc_page();

//===----------------------------------------------------------------------===//
// Range class for memory ranges

struct range {
    range(uint64_t begin, uint64_t end) : begin{begin}, end{end} { }
    uint64_t begin;
    uint64_t end;
};

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
