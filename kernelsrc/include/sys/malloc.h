/*
 * File:   malloc.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 04-Aug-2017 05:45:29 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 12-Aug-2017 10:01:16 PM
*/

#pragma once

#include "system.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct DList DList;
struct DList
{
    DList *next;
    DList *prev;
};

typedef struct Chunk Chunk;
struct Chunk
{
    DList all;
    int used;
    union
    {
        char data[0];
        DList free;
    };
    uint32_t magic;
};

#define CONTAINER(C, l, v) ((C *) (((char *) v) - (intptr_t) &(((C *) 0) -> l)))
#define OFFSETOF(TYPE, MEMBER)  __builtin_offsetof(TYPE, MEMBER)

#define DLIST_INIT(v, l) dlist_init(&v -> l)

#define DLIST_REMOVE_FROM(h, d, l)                                          \
    {                                                                       \
        typeof(**h) * *h_ = h, *d_ = d;                                     \
        DList *head = &(*h_) -> l;                                          \
        dlist_remove_from(&head, &d_ -> l);                                 \
        if (head == NULL) {                                                 \
            *h_ = NULL;                                                     \
        } else {                                                            \
            *h_ = CONTAINER(typeof(**h), l, head);                          \
        }                                                                   \
    }

#define DLIST_PUSH(h, v, l)                                                 \
    {                                                                       \
        typeof(*v) * *h_ = h, *v_ = v;                                      \
        DList *head = &(*h_) -> l;                                          \
        if (*h_ == NULL) head = NULL;                                       \
        dlist_push(&head, &v_ -> l);                                        \
        *h_ = CONTAINER(typeof(*v), l, head);                               \
    }

#define DLIST_POP(h, l)                                                     \
    ({                                                                      \
        typeof(**h) * *h_ = h;                                              \
        DList *head = &(*h_) -> l;                                          \
        DList *res = dlist_pop(&head);                                      \
        if (head == NULL) {                                                 \
            *h_ = NULL;                                                     \
        } else {                                                            \
            *h_ = CONTAINER(typeof(**h), l, head);                          \
        }                                                                   \
        CONTAINER(typeof(**h), l, res);                                     \
    })

#define DLIST_ITERATOR_BEGIN(h, l, it)                                                  \
    {                                                                                   \
        typeof(*h) * h_ = h;                                                            \
        DList *last_ ## it = h_ -> l . prev, *iter_ ## it = &h_ -> l, *next_ ## it;     \
        do {                                                                            \
            if (iter_ ## it == last_ ## it) {                                           \
                next_ ## it = NULL;                                                     \
            } else {                                                                    \
                next_ ## it = iter_ ## it -> next;                                      \
            }                                                                           \
            typeof(*h) * it = CONTAINER(typeof(*h), l, iter_ ## it);

#define DLIST_ITERATOR_END(it)                                                          \
    } while ((iter_ ## it = next_ ## it)) ;                                             \
    }

#define DLIST_ITERATOR_REMOVE_FROM(h, it, l) DLIST_REMOVE_FROM(h, iter_ ## it, l)

enum
{
    NUM_SIZES = 32,
    ALIGN = 4,
    MIN_SIZE = sizeof(DList),
    HEADER_SIZE = OFFSETOF(Chunk, data),
};

void* malloc(size_t size);
void  free(void *mem);
void  mrvn_memory_init(void *mem, size_t size);

extern size_t malloc_mem_free;
extern size_t malloc_mem_used;

#ifdef __cplusplus
}
#endif
