#include <bst.h>
#include <stdio.h>
#include <stdlib.h>

#define MINIMUM_KEY(minimum, bst_node) \
	do \
	{ \
		minimum = bst_node; \
		while (minimum && minimum->left) minimum = minimum->left; \
	} while(0);

struct tree_node_t
{
	int key;
	struct tree_node_t* left;
	struct tree_node_t* right;
};

// Inserisce in ABR
int bst_insert(struct tree_node_t** root, int key)
{
	if ( *root == NULL )
	{
		struct tree_node_t* leaf = (struct tree_node_t*) malloc(sizeof(struct tree_node_t));
		if (!leaf) return -1;
		leaf->key = key;
		leaf->left = NULL;
		leaf->right = NULL;
		*root = leaf;
		return 0;
	}
	else
	{
		if ( key < (*root)->key )
			return bst_insert(&((*root)->left), key);
		else 
		{
			if ( key > (*root)->key )
				return bst_insert(&((*root)->right), key);
			else return -1;
		}
	}
}

// Cerca in un ABR
int bst_find(const struct tree_node_t* root, int key)
{
	if ( root == NULL ) return 0;
	else
	{
		if ( root->key > key ) return bst_find(root->left, key);
		else 
			if ( root->key == key ) return 1;
			else return bst_find(root->right, key); 
	}

}

// Visita simmetrica
void inorder_visit(const struct tree_node_t* root)
{
	if ( root != NULL )
	{
		inorder_visit(root->left);
		printf("%d ", root->key);
		inorder_visit(root->right);
	}
}

struct tree_node_t* bst_delete_node(struct tree_node_t* root, int key)
{
	if (root == NULL) return NULL;
	if (key < root->key)
		root->left = bst_delete_node(root->left, key);
 

	else if (key > root->key)
		root->right = bst_delete_node(root->right, key);

	else
	{
		if (root->left == NULL)
		{
			struct tree_node_t* temp = root->right;
			free(root);
			return temp;
		}
		else if (root->right == NULL)
		{
			struct tree_node_t* temp = root->left;
			free(root);
			return temp;
		}
		struct tree_node_t* temp;
		MINIMUM_KEY(temp, root->left);
		root->key = temp->key;
		root->right = bst_delete_node(root->right, temp->key);
	}
	return root;
}

// Libera la memoria occupata dall'ABR
void bst_free(struct tree_node_t** root)
{
	if ( *root != NULL )
	{
		bst_free(&((*root)->left));
		bst_free(&((*root)->right));
		free(*root);
	}
}

/* int main()
{
	size_t dim;
	scanf("%lu", &dim);
	if ( dim == 0 ) return 0;
	bst_free(&tree);
	return 0;
} */