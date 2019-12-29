/**
 * Copyright (c) 2018 The cxkernel Authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 *
 * @file   linked_list.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on December 15 2018, 7:37 PM
 */

#pragma once

#include "common.h"

__BEGIN_CDECLS

typedef struct list_node
{
    struct list_node* prev;
    struct list_node* next;
} list_node_t;
typedef struct list_node list_head_t;

#define INIT_LLIST(ptr) {(ptr)->next=(NULL);(ptr)->prev=(ptr);}
#define LIST_ENTRY(ptr, type, member) container_of(ptr, type, member)

/**
 * Iterates through each list of a list
 * @param  counter The current list pointer
 * @param  head    A list of the list
 */
#define foreach_llist(counter, head) \
        for(counter = (head); counter != NULL; counter = counter -> next)

/**
 * Iterates through each entry of the list
 * @param  counter The pointer to the current iterated structure
 * @param  member  The name of the list in the structure
 * @param  head    The pointer to the structure of the list
 */
#define foreach_llist_entry(counter, member, head) \
        for(counter = LIST_ENTRY(head, TYPEOF(*counter), member); \
            counter != NULL; counter = counter -> member.next == NULL ? \
            NULL : LIST_ENTRY(counter -> member.next, TYPEOF(*counter), member))

/**
 * Same as foreach_llist, but with a NULL check.
 */
#define foreach_llist_safe(c, n, head) \
        for(list_t *c = (head), *n = c -> next; n != NULL; c = n, n = c -> next)

static inline list_node_t* list_begin(list_node_t* a)
{
    if(a == NULL) return a;
    while(a -> prev != a) a = a -> prev;
    return a;
}

static inline list_node_t* list_end(list_node_t* a)
{
    if(a == NULL) return a;
    while(a -> next != NULL) a = a -> next;
    return a;
}

static inline bool list_isempty(list_node_t* a)
{
    if(a == NULL) return true;
    list_node_t* front = list_begin(a);
    return front -> next == NULL && front -> prev == front;
}

static inline int list_count(list_node_t* a)
{
    if(a == NULL) return 0;
    register int ctr = 0;
    list_node_t* front = list_begin(a);
    while(front -> next != NULL) front = front -> next, ctr++;
    return ctr;
}

static inline void list_append(list_head_t* head, list_node_t* node)
{
    if(head -> next != NULL)
        head -> next -> prev = node;
    node -> next = head -> next;
    node -> prev = head;
    head -> next = node;
}

static inline void list_insert(list_head_t* head, list_head_t* node)
{
    node -> next = head;
    node -> prev = head -> prev;
    if(head -> prev == head)
        node -> prev = node;
    else
        head -> prev -> next = node;
    head -> prev = node;
}

static inline void list_push_front(list_head_t* head, list_head_t* list)
{
    list_node_t* s = list_begin(list);
    list_node_t* t = list_end(list);
    s -> prev = head;
    t -> next = head -> next;
    head -> next = s;
}

static inline list_node_t* list_remove(list_node_t* node)
{
    node->prev->next = node->next;
    if(node->next) node->next->prev = node->prev;
    return node;
}

__END_CDECLS
