/**
 * @file   tree.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Monday, July 9th 2018, 1:39:06 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-07-20T14:30:42-04:00
 */

#pragma once

#include "common.h"
#include "list.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct tree_node
{
    void* value;
    list_t* children;
    struct tree_node* parent;
} tree_node_t;

typedef struct
{
    size_t nodes;
    struct tree_node* root;
} tree_t;

#ifdef __cplusplus
}
#endif
