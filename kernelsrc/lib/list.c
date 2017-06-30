#include "lib/list.h"
#include "memory/kheap.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

list_t* list_create()
{
    list_t * out = kmalloc(kheap, sizeof(list_t));
    out -> head = NULL;
    out -> tail = NULL;
    out -> length = 0;
    return out;
}

void list_destroy(list_t* list)
{
    list_node_t* n = list -> head;
    while (n)
    {
        list_node_t* s = n -> next;
        kfree(kheap, n);
        n = s;
    }
}

void list_append(list_t* list, list_node_t* item)
{
    item -> next = NULL;
    if (!list -> length) // List is empty!
    {
        item -> prev = NULL;
        list -> head = item;
        list -> tail = item;
        list -> length++;
        return;
    }

    // Append item to the end of list and set the item's previous node to the tail of the list
    list -> tail -> next = item;
    item -> prev = list -> tail;
    list -> tail = item; // Finally set the list's tail to the item we just appended
}

void list_merge(list_t* target, list_t* source)
{
    if (source -> head)
        source -> head -> prev = target -> tail;

    if (target -> tail)
        target -> tail -> next = source -> head;
    else
        target -> head = source -> head;

    if (source -> tail)
        target -> tail = source -> tail;

    target -> length += source -> length;

    kfree(kheap, source);
}

void list_remove(list_t* list, size_t index)
{
}

void list_insert_after(list_t* list, list_node_t* item, size_t index)
{
}

int list_get_index(list_t* list, void* value)
{
    int i = 0;
    foreach(item, list)
    {
        if (item -> data == value)
            return i;
        i++;
    }
    return -1;
}

list_node_t* list_create_node(void* item)
{
    list_node_t* ret = kmalloc(kheap, sizeof(list_node_t));
    ret -> prev = NULL;
    ret -> next = NULL;
    ret -> data = item;
    return ret;
}
