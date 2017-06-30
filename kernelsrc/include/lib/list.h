/*
 * File:   list.h
 * Author: Kevin
 *
 * Created on June 25, 2017, 2:52 PM
*/

#include "system.h"

#ifndef LIST_H
#define LIST_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct list_node
{
    struct list_node* prev;
    struct list_node* next;
    void* data;
} list_node_t;

typedef struct
{
    list_node_t* head;
    list_node_t* tail;
    size_t length;
} list_t;

void list_destroy(list_t* list);
void list_append(list_t* list, list_node_t* item);
void list_merge(list_t* target, list_t* source);
void list_remove(list_t* list, size_t index);
void list_insert_after(list_t* list, list_node_t* item, size_t index);

list_node_t* list_create_node(void* item);
list_t* list_create();
int list_get_index(list_t*, void*);

#define foreach(i, list) for (list_node_t * i = (list)->head; i != NULL; i = i->next)
#define foreachr(i, list) for (list_node_t * i = (list)->tail; i != NULL; i = i->prev)

#ifdef __cplusplus
}
#endif

#endif /* LIST_H */
