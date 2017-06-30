/*
 * File:   tree.h
 * Author: Kevin
 *
 * Created on June 25, 2017, 2:51 PM
*/

#include "system.h"
#include "lib/list.h"

#ifndef TREE_H
#define TREE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct tree_node
{
    void* data;
    list_t* children;
    struct tree_node* parent;
} tree_node_t;

typedef struct
{
    size_t nodes;
    tree_node_t* root;
} tree_t;

tree_t* tree_create();
void tree_destroy(tree_t* tree);
void tree_remove(tree_t* tree, tree_node_t* node);
void tree_insert(tree_t* tree, tree_node_t* parent, tree_node_t* node);
tree_node_t* tree_node_create(void* value);

#ifdef __cplusplus
}
#endif

#endif /* TREE_H */
