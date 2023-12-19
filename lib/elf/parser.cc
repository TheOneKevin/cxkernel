#include "elf.h"

// Determines whether a given elf header is valid.
static inline bool is_valid_elf(elf32_ehdr_t* h) {
    return
        (h -> e_ident[EI_MAG0] == ELFMAG0) &&
        (h -> e_ident[EI_MAG1] == ELFMAG1) &&
        (h -> e_ident[EI_MAG2] == ELFMAG2) &&
        (h -> e_ident[EI_MAG3] == ELFMAG3);
}

Result<void> elf::Context::load(Context& ctx, void* img) {
    auto* hdr = static_cast<elf32_ehdr_t*>(img);
    if(!is_valid_elf(hdr)) {
        return E::INVALID;
    } else if(hdr -> e_ident[EI_CLASS] == ELFCLASS32) {
        ctx.is_64bits_ = false;
        return ctx.impl32_.load(hdr);
    } else if(hdr -> e_ident[EI_CLASS] == ELFCLASS64) {
        ctx.is_64bits_ = true;
        return ctx.impl64_.load(static_cast<elf64_ehdr_t*>(img));
    } else {
        return E::INVALID;
    }
}
