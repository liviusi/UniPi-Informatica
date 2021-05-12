#ifndef _BST_H_
#define _BST_H_

struct tree_node_t;

int bst_insert(struct tree_node_t**, int);

int bst_find(const struct tree_node_t*, int);

void inorder_visit(const struct tree_node_t*);

struct tree_node_t* bst_delete_node(struct tree_node_t*, int);

void bst_free(struct tree_node_t**);

#endif