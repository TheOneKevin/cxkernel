#include "lib/tree.h"
#include "memory/kheap.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

tree_t* tree_create()
{
    tree_t* ret = kmalloc(sizeof(tree_t));
    ret -> nodes = 0;
    ret -> root = NULL;
    return ret;
}

void tree_destroy(tree_t* tree)
{
    //TODO: I got lazy and I have a headache
}

// Remove the node and move its children into the parent
void tree_remove(tree_t* tree, tree_node_t* node)
{
    tree_node_t* parent = node -> parent;
    if (!parent)
        return;         // This node is the root node.
    list_remove(parent -> children, list_get_index(parent -> children, node));
    list_merge(parent -> children, node -> children); // Merge already frees the list for us, but not the contents
    kfree(node);
}

void tree_insert(tree_t* tree, tree_node_t* parent, tree_node_t* node)
{
    list_append(parent -> children, list_create_node(node));
    node -> parent = parent;
    tree -> nodes++;
}

tree_node_t* tree_node_create(void* value)
{
    tree_node_t* ret = kmalloc(sizeof(tree_node_t));
    ret -> data = value;
    ret -> children = list_create();
    ret -> parent = NULL;
    return ret;
}
