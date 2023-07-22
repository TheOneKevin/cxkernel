#include "elf.h"

using N = elf::Context;

status_t N::Context32::load(elf32_ehdr_t *hdr) {
    (void) hdr;
    (void) img;
    (void) shdr;
    (void) phdr;
    (void) symtab;
    (void) strtab;
    return E::NOT_IMPLEMENTED;
}
