#ifndef _BST_H_
#define _BST_H_

#include <stdbool.h>

struct tree_node_t;

int bstInsert(struct tree_node_t**, int);
bool bstFind(const struct tree_node_t*, int);
void bstInorderVisit(const struct tree_node_t*);
struct tree_node_t* bstDeleteNode(struct tree_node_t*, int);
void bstFree(struct tree_node_t**);

#endif