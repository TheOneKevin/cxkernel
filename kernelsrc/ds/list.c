/**
 * File:   list.c
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 * 
 * @date Created on Monday, July 9th 2018, 11:51:28 am
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-07-26T09:18:26-04:00
 */

#include "list.h"
#include "mm/malloc.h"

/* =============================== Internal functions =============================== */

/**
 * Internal function that deletes an entry by making
 * the next point to prev and prev point to next.
 */
static inline void __list_del(list_t* prev, list_t* next)
{
    if(prev != NULL) prev -> next = next;
    if(next != NULL) next -> prev = prev;
}

/**
 * Internal function that adds an entry between the next and
 * prev list heads.
 */
static inline void __list_add(list_t* list, list_t* prev, list_t* next)
{
    if(prev != NULL) prev -> next = list;
    if(next != NULL) next -> prev = list;
    list -> next = next;
    list -> prev = prev;
}

/* =============================== External functinons =============================== */

list_t* create_list(void)
{
    list_t* newList = (list_t*) malloc(sizeof(list_t));
    INIT_list(newList);
    return newList;
}

list_t* list_get_nth(list_t* root, int n)
{
    list_t* ret = root;
    for(int i = 0; i < n; i++)
        ret = ret -> next;
    return ret;
}

inline void list_del(list_t* entry)
{
    __list_del(entry -> prev, entry -> next);
    entry -> prev = NULL;
    entry -> next = NULL;
}

inline void list_add(list_t* entry, list_t* head)
{
    __list_add(entry, head, head -> next);
}

inline void list_add_tail(list_t* entry, list_t* head)
{
    __list_add(entry, head -> prev, head);
}

inline bool clist_is_empty(list_t* entry)
{
    return entry -> next == entry;
}

inline void clist_del_init(list_t* entry)
{
    __list_del(entry -> prev, entry -> next);
    INIT_Clist(entry);
}

inline bool list_is_empty(list_t* entry)
{
    return entry -> next == NULL;
}

inline void list_del_init(list_t* entry)
{
    __list_del(entry -> prev, entry -> next);
    INIT_list(entry);
}

inline list_t* list_get_first(list_t* entry)
{
    list_t* ret = entry;
    while(ret -> prev != NULL) ret = ret -> prev;
    return ret;
}

inline list_t* list_get_last(list_t* entry)
{
    list_t* ret = entry;
    while(ret -> next != NULL) ret = ret -> next;
    return ret;
}
