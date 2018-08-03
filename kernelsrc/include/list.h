/**
 * @file   list.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Sunday, November 26th 2017, 9:42:00 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-07-09T13:53:26-04:00
 * 
 * This file implements two types of linked lists: circularly linked and regular. Circlularly linked
 * lists are abbreviated as "clist" while regular lists are "list."
 * Circularly linked lists has the last element reference the first element while in regular linked lists,
 * the last element references a NULL pointer thus terminating the list.
 */

#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct list
{
    struct list *prev, *next;
} list_t;

//! Gets the base pointer of the structure that contains the list head
#define LIST_ENTRY(ptr, type, member) container_of(ptr, type, member)
//! Makes the list head's last node reference the first one (circular)
#define INIT_Clist(ptr) do { (ptr) -> next = (ptr); (ptr) -> prev = (ptr); } while(0);
//! Makes new structure of a circularly linked list head
#define NEW_Clist(ptr) { &(ptr), &(ptr) }
//! Terminates the list's last entry by referencing it to NULL
#define INIT_list(ptr) do { (ptr) -> next = (NULL); (ptr) -> prev = (ptr); } while(0);
//! Makes new structure of a list head
#define NEW_list() { (NULL), (NULL) }

/**
 * Iterates through each list of a circularly linked list
 * @param  counter The current list pointer
 * @param  head    A list of the list
 */
#define foreach_clist(counter, head) \
        for((counter) = (head) -> next; (counter) != (head); (counter) = (counter) -> next)
/**
 * Iterates through each entry of the circularly linked list
 * @param  counter The pointer to the current iterated structure
 * @param  list    The pointer to the a structure of the list
 * @param  member  The name of the list in the structure
 */
#define foreach_clist_entry(counter, list, member) \
        for((counter) = LIST_ENTRY((list) -> (member) -> next, typeof(list), member); \
            (counter) -> (member) != (list) -> (member); \
            (counter) = LIST_ENTRY((counter) -> (member) -> next, typeof(list), member))

/**
 * Iterates through each list of a list
 * @param  counter The current list pointer
 * @param  head    A list of the list
 */
#define foreach_list(counter, head) \
        for(counter = (head); counter -> next != NULL; counter = counter -> next)
/**
 * Iterates through each entry of the list
 * @param  counter The pointer to the current iterated structure
 * @param  list    The pointer to the a structure of the list
 * @param  member  The name of the list in the structure
 */
#define foreach_list_entry(counter, member, list) \
        for(counter = LIST_ENTRY(&((list) -> member), typeof(*counter), member); \
            counter -> member.next != NULL; \
            counter = LIST_ENTRY(counter -> member.next, typeof(*counter), member))

#define foreach_list_safe(c, n, head) \
        for(list_t *c = (head), *n = c -> next; n != NULL; c = n, n = c -> next)

//! Imperative foreach loops
//!@{
#define iforeach_list_entry(x) foreach_list_entry(x)
#define iforeach_list(x) foreach_list(x)
#define iforeach_clist_entry(x) foreach_clist_entry(x)
#define iforeach_clist(x) foreach_clist(x)
//!@}

/**
 * Creates a list
 * @return List head element
 */
list_t* create_list(void);

/**
 * Deletes an entry from a list. Does not free any memory
 * associated with the list. Entry is in undefined state
 * @param entry List entry to free.
 */
void list_del(list_t* entry);

/**
 * Inserts an entry to the list AFTER a specified entry
 * @param entry The entry to be added
 * @param head  The list head to add the entry after
 */
void list_add(list_t* entry, list_t* head);

/**
 * Inserts an entry to the list BEFORE a specified entry
 * @param entry The entry to be added
 * @param head  The list head to add the entry before
 */
void list_add_tail(list_t* entry, list_t* head);

/**
 * Checks if a circularly list head is the sole element in the list
 * @param  entry List head
 * @return True or false
 */
bool clist_is_empty(list_t* entry);

/**
 * Deletes an entry from a circularly linked list and
 * reinitializes it with the pointer values
 * @param entry [description]
 */
void clist_del_init(list_t* entry);

/**
 * Checks if a list head is the sole element in the list
 * @param  entry List head
 * @return True or false
 */
bool list_is_empty(list_t* entry);

/**
 * Deletes an entry from a linked list and
 * reinitializes it with the pointer values
 * @param entry [description]
 */
void list_del_init(list_t* entry);

/**
 * Gets the first node in a linked list
 * @param  entry An entry of the list
 * @return       Pointer to the first list head
 */
list_t* list_get_first(list_t* entry);

/**
 * Gets the last node in a linked list
 * @param  entry An entry of the list
 * @return       Pointer to the last list head
 */
list_t* list_get_last(list_t* entry);

#ifdef __cplusplus
}
#endif
