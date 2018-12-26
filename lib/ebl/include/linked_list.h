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

static inline list_node_t* list_begin(list_node_t* a)
{
    while(a -> prev != a) a = a -> prev;
    return a;
}

static inline list_node_t* list_end(list_node_t* a)
{
    while(a -> next != NULL) a = a -> next;
    return a;
}

static inline void list_append(list_head_t* head, list_node_t* node)
{
    if(head -> next != NULL)
        head -> next -> prev = node;
    node -> next = head->next;
    node -> prev = head;
    head -> next = node;
}

static inline void list_insert(list_head_t* head, list_head_t* list)
{
    list_node_t* s = list_begin(list);
    list_node_t* t = list_end(list);
    s -> prev = head;
    t -> next = head -> next;
    head -> next = s;
}

__END_CDECLS
