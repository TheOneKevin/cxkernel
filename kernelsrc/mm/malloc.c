/*
 * Filename: malloc.c
 * Author:   Goswin von Brederlow
 * Email:    goswin-v-b@web.de
 *
 * Created on 04-Aug-2017 05:45:03 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 13-Aug-2017 07:44:16 PM
 */

#include "sys/malloc.h"
#include "lib/assert.h"

#define NUM_SLOTS 1024
void *slot[NUM_SLOTS] = { NULL };
Chunk *free_chunk[NUM_SIZES] = { NULL };
size_t malloc_mem_free = 0;
size_t malloc_mem_used = 0;
size_t mem_meta = 0;
Chunk *first = NULL;
Chunk *last = NULL;

// initialize a one element DList
static inline void dlist_init(DList *dlist)
{
    //printf("%s(%p)\n", __FUNCTION__, dlist);
    dlist -> next = dlist;
    dlist -> prev = dlist;
}

// insert d2 after d1
static inline void dlist_insert_after(DList *d1, DList *d2)
{
    //printf("%s(%p, %p)\n", __FUNCTION__, d1, d2);
    DList *n1 = d1 -> next;
    DList *e2 = d2 -> prev;

    d1 -> next = d2;
    d2 -> prev = d1;
    e2 -> next = n1;
    n1 -> prev = e2;
}

// insert d2 before d1
static inline void dlist_insert_before(DList *d1, DList *d2)
{
    //printf("%s(%p, %p)\n", __FUNCTION__, d1, d2);
    DList *e1 = d1 -> prev;
    DList *e2 = d2 -> prev;

    e1 -> next = d2;
    d2 -> prev = e1;
    e2 -> next = d1;
    d1 -> prev = e2;
}

// remove d from the list
static inline void dlist_remove(DList *d)
{
    //printf("%s(%p)\n", __FUNCTION__, d);
    d -> prev -> next = d -> next;
    d -> next -> prev = d -> prev;
    d -> next = d;
    d -> prev = d;
}

// push d2 to the front of the d1p list
static inline void dlist_push(DList **d1p, DList *d2)
{
    //printf("%s(%p, %p)\n", __FUNCTION__, d1p, d2);
    if (*d1p != NULL)
    {
        dlist_insert_before(*d1p, d2);
    }
    *d1p = d2;
}

// pop the front of the dp list
static inline DList * dlist_pop(DList **dp)
{
    //printf("%s(%p)\n", __FUNCTION__, dp);
    DList *d1 = *dp;
    DList *d2 = d1 -> next;
    dlist_remove(d1);
    if (d1 == d2)
    {
        *dp = NULL;
    }
    else
    {
        *dp = d2;
    }
    return d1;
}

// remove d2 from the list, advancing d1p if needed
static inline void dlist_remove_from(DList **d1p, DList *d2)
{
    //printf("%s(%p, %p)\n", __FUNCTION__, d1p, d2);
    if (*d1p == d2)
    {
        dlist_pop(d1p);
    }
    else
    {
        dlist_remove(d2);
    }
}

static void memory_chunk_init(Chunk *chunk)
{
    //printf("%s(%p)\n", __FUNCTION__, chunk);
    DLIST_INIT(chunk, all);
    chunk -> used = 0;
    DLIST_INIT(chunk, free);
}

static size_t memory_chunk_size(const Chunk *chunk)
{
    //printf("%s(%p)\n", __FUNCTION__, chunk);
    char *end = (char *) (chunk -> all . next);
    char *start = (char *) (&chunk -> all);
    return (end - start) - HEADER_SIZE;
}

static int memory_chunk_slot(size_t size)
{
    int n = -1;
    while (size > 0)
    {
        ++n;
        size /= 2;
    }
    return n;
}

void mrvn_memory_init(void *mem, size_t size)
{
    char *mem_start = (char *) (((intptr_t) mem + ALIGN - 1) & (~(ALIGN - 1)));
    char *mem_end = (char *) (((intptr_t) mem + size) & (~(ALIGN - 1)));
    first = (Chunk *) mem_start;
    Chunk *second = first + 1;
    last = ((Chunk *) mem_end) - 1;
    memory_chunk_init(first);
    memory_chunk_init(second);
    memory_chunk_init(last);
    dlist_insert_after(&first -> all, &second -> all);
    dlist_insert_after(&second -> all, &last -> all);
    // make first/last as used so they never get merged
    first -> used = 1;
    last -> used = 1;

    size_t len = memory_chunk_size(second);
    int n = memory_chunk_slot(len);
    //printf("%s(%p, %#lx) : adding chunk %#lx [%d]\n", __FUNCTION__, mem, size, len, n);
    DLIST_PUSH(&free_chunk[n], second, free);
    malloc_mem_free = len - HEADER_SIZE;
    mem_meta = sizeof(Chunk) * 2 + HEADER_SIZE;
}

void *malloc(size_t size)
{
    //printf("%s(%#lx)\n", __FUNCTION__, size);
    size = (size + ALIGN - 1) & (~(ALIGN - 1));

    if (size < MIN_SIZE)
        size = MIN_SIZE;

    int n = memory_chunk_slot(size - 1) + 1;

    if (n >= NUM_SIZES)
        return NULL;

    while (!free_chunk[n])
    {
        ++n;
        if (n >= NUM_SIZES)
            return NULL;
    }

    Chunk *chunk = DLIST_POP(&free_chunk[n], free);
    chunk -> magic = 0xCAFEBABE;
    size_t size2 = memory_chunk_size(chunk);
    //printf("@ %p [%#lx]\n", chunk, size2);
    size_t len = 0;

    if (size + sizeof(Chunk) <= size2)
    {
        Chunk *chunk2 = (Chunk *) (((char *) chunk) + HEADER_SIZE + size);
        chunk2 -> magic = 0xCAFEBABE;
        memory_chunk_init(chunk2);
        dlist_insert_after(&chunk -> all, &chunk2 -> all);
        len = memory_chunk_size(chunk2);
        int n = memory_chunk_slot(len);
        //printf("  adding chunk @ %p %#lx [%d]\n", chunk2, len, n);
        DLIST_PUSH(&free_chunk[n], chunk2, free);
        mem_meta += HEADER_SIZE;
        malloc_mem_free += len - HEADER_SIZE;
    }

    chunk -> used = 1;
    //memset(chunk->data, 0xAA, size);
    //printf("AAAA\n");
    malloc_mem_free -= size2;
    malloc_mem_used += size2 - len - HEADER_SIZE;
    //printf("  = %p [%p]\n", chunk->data, chunk);
    return chunk -> data;
}

static void remove_free(Chunk *chunk)
{
    size_t len = memory_chunk_size(chunk);
    int n = memory_chunk_slot(len);
    //printf("%s(%p) : removing chunk %#lx [%d]\n", __FUNCTION__, chunk, len, n);
    DLIST_REMOVE_FROM(&free_chunk[n], chunk, free);
    malloc_mem_free -= len - HEADER_SIZE;
}

static void push_free(Chunk *chunk)
{
    size_t len = memory_chunk_size(chunk);
    int n = memory_chunk_slot(len);
    //printf("%s(%p) : adding chunk %#lx [%d]\n", __FUNCTION__, chunk, len, n);
    DLIST_PUSH(&free_chunk[n], chunk, free);
    malloc_mem_free += len - HEADER_SIZE;
}

void free(void *mem)
{
    if(mem == NULL) return;

    Chunk *chunk = (Chunk *) ((char *) mem - HEADER_SIZE);
    Chunk *next = CONTAINER(Chunk, all, chunk -> all . next);
    Chunk *prev = CONTAINER(Chunk, all, chunk -> all . prev);

    if(chunk -> magic != 0xCAFEBABE || chunk -> used != 1) return;

    //printf("%s(%p): @%p %#lx [%d]\n", __FUNCTION__, mem, chunk, memory_chunk_size(chunk), memory_chunk_slot(memory_chunk_size(chunk)));
    malloc_mem_used -= memory_chunk_size(chunk);

    if (next -> used == 0)
    {
        // merge in next
        remove_free(next);
        dlist_remove(&next -> all);
        //memset(next, 0xDD, sizeof(Chunk));
        mem_meta -= HEADER_SIZE;
        malloc_mem_free += HEADER_SIZE;
    }
    if (prev -> used == 0)
    {
        // merge to prev
        remove_free(prev);
        dlist_remove(&chunk -> all);
        //memset(chunk, 0xDD, sizeof(Chunk));
        push_free(prev);
        mem_meta -= HEADER_SIZE;
        malloc_mem_free += HEADER_SIZE;
    }
    else
    {
        // make chunk as free
        chunk -> used = 0;
        DLIST_INIT(chunk, free);
        push_free(chunk);
    }
}

void __malloc_check()
{
    int i;
    Chunk *t = last;
    DLIST_ITERATOR_BEGIN(first, all, it)
    {
        ASSERT(CONTAINER(Chunk, all, it -> all . prev) == t, "");
        t = it;
    } DLIST_ITERATOR_END(it);

    for (i = 0; i < NUM_SIZES; ++i)
    {
        if (free_chunk[i])
        {
            t = CONTAINER(Chunk, free, free_chunk[i] -> free . prev);
            DLIST_ITERATOR_BEGIN(free_chunk[i], free, it)
            {
                ASSERT(CONTAINER(Chunk, free, it -> free . prev) == t, "");
                t = it;
            } DLIST_ITERATOR_END(it);
        }
    }
}
