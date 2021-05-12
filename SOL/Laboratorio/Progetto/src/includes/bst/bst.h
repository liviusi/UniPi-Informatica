#ifndef _BST_H_
#define _BST_H_

#include <stdbool.h>

// Struct fields are not exposed in order to maintain BST invariant properties.
// Duplicates are not allowed in this implementation.
typedef struct tree_node tree_node_t;

int bstInsert(tree_node_t**, int);
bool bstFind(const tree_node_t*, int);
void bstInorderVisit(const tree_node_t*);
tree_node_t* bstDeleteNode(tree_node_t*, int);
void bstFree(tree_node_t*);

#endif