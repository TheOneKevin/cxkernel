Memory map for upper 1 GB:

```
CX_PAGE_TABLES_END    +----------------------+ 0xFFFF`FFFF ^
                      | Top Level Page Dir   |             |
                      +----------------------+ 0xFFFF`F000 |
                      |                      |             | 8
                      | Page Tables          |             |
                      |                      |             | M
CX_PAGE_TABLES_BEGIN  +----------------------+ 0xFFC0`0000 | i
                      |                      |             | B
                      | PAEx Tables          |             |
CX_PAGE_TABLES_BEGIN  |                      |             |
CX_STACK_BOTTOM       +----------------------+ 0xFF80`0000 V
                      | Kernel Stack         |             ^
CX_STACK_TOP          +----------------------+             |
                       Unmapped Space                      |
CX_STACK_LIMIT        +----------------------+             |
                       Unmapped Space                      |
CX_PFN_DATABASE_END   +----------------------+             |
                      |                      |             | 5
                      | PFN Database         |             | 0
                      |                      |             | 4
CX_PFN_DATABASE_BEGIN +----------------------+             |
CX_IMAGE_END          | +------------------+ |             | M
                      | |                  | |             | i
                      | | Kernel Data (rw) | |             | B
                      | |                  | |             |
                      | +------------------+ |             |
                      | +------------------+ |             |
                      | |                  | |             |
                      | | Kernel Text (rx) | |             |
                      | |                  | |             |
                      | +------------------+ |             |
CX_IMAGE_BASE         +----------------------+ 0xE000`0000 v
                        Unmapped Space                     ^
CX_SLAB_BREAK         +----------------------+             | 5
                      |                      |             | 1
                      |                      |             | 2
                      |                      |             |
                      | Kernel Slab Space    |             | M
                      |                      |             | i
                      |                      |             | B
                      |                      |             |
CX_SLAB_BASE          +----------------------+ 0xC000`0000 v
```