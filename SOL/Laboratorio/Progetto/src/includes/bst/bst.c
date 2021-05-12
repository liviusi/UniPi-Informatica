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
int bstInsert(struct tree_node_t** root, int key)
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
			return bstInsert(&((*root)->left), key);
		else 
		{
			if ( key > (*root)->key )
				return bstInsert(&((*root)->right), key);
			else return -1;
		}
	}
}

// Cerca in un ABR
bool bstFind(const struct tree_node_t* root, int key)
{
	if ( root == NULL ) return false;
	else
	{
		if ( root->key > key ) return bstFind(root->left, key);
		else 
			if ( root->key == key ) return true;
			else return bstFind(root->right, key); 
	}

}

// Visita simmetrica
void bstInorderVisit(const struct tree_node_t* root)
{
	if ( root != NULL )
	{
		bstInorderVisit(root->left);
		printf("%d ", root->key);
		bstInorderVisit(root->right);
	}
}

struct tree_node_t* bstDeleteNode(struct tree_node_t* root, int key)
{
	if (root == NULL) return NULL;
	if (key < root->key)
		root->left = bstDeleteNode(root->left, key);
 

	else if (key > root->key)
		root->right = bstDeleteNode(root->right, key);

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
		root->right = bstDeleteNode(root->right, temp->key);
	}
	return root;
}

// Libera la memoria occupata dall'ABR
void bstFree(struct tree_node_t** root)
{
	if ( *root != NULL )
	{
		bstFree(&((*root)->left));
		bstFree(&((*root)->right));
		free(*root);
	}
}

/* int main()
{
	size_t dim;
	scanf("%lu", &dim);
	if ( dim == 0 ) return 0;
	bstFree(&tree);
	return 0;
} */