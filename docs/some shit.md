 Ordered array of pages:
 [][][][][][][][][][][][][][]
 ^---FREE BLOCK---^
 
 **!ALLOC!**
 
 [][][][][][][][][][][][][][]
     ^------------^
 
 (Free block is always shrunk at front)
 
 Rough sketch of algorithm:
 
 Ordered linked list of all free blocks = BLKS
 Alloc(size of allocation = SZ):
    Traverse BLKS:
        Find best fit free block from BLKS
    Let BEST = best fit free block found
    Split BEST.pages after SZ pages
    (Return the head of the first half after split)
    Let BEST.pages = the head of the second half after split
    Shrink BEST by SZ
 
 **!FREE!**
 
 (Case 1, no merge)
 [][][][][][][][][][][][][][]
     ^------------^    ^----^
                        Allocate a new structure for this
                 +------->
                Now the 2 blocks are linked (forms a list)
 
 (Case 2, with merge)
 [][][][][][][][][][][][][][]
     ^------------^^----^^---
                   +----+
                   This is the newly freed block
 
 Rough sketch of algorithm:
 
 Ordered linked list of all free blocks = BLKS
 Current block = CUR
 
 Free(linked list of pages = PAGES):
    Allocate a new block for CUR
    Fill in the information for CUR based on PAGES
    Insert CUR into BLKS
    IF CUR.node.prev immediately preceeds CUR
        Expand CUR.node.prev by CUR.size
        Link all CUR.node.prev.pages with CUR.pages
        Transfer any data from CUR to CUR.node.prev
        (Linked list) remove CUR from BLKS
        let TMP = CUR.node.prev (use pointers)
        Free CUR
        CUR is now TMP (use pointers)
    IF CUR.node.next immediately follows CUR
        EXPAND CUR by CUR.node.next.size
        Link all CUR.pages with CUR.node.next.pages
        Transfer any data from CUR.node.next to CUR
        (Linked list) remove CUR.node.next from BLKS
        Free CUR.node.next

 Ordered List Algorithm:
 
 Let SZ be the total size of linked list
 Let LIST be the linked list
 
 
