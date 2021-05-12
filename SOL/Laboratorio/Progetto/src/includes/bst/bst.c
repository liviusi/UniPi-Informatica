#include <bst.h>
#include <stdio.h>
#include <stdlib.h>

#define MINIMUM_KEY(minimum, bst_node) \
	do \
	{ \
		minimum = bst_node; \
		while (minimum && minimum->left) minimum = minimum->left; \
	} while(0);

struct tree_node
{
	int key;
	struct tree_node* left;
	struct tree_node* right;
};

/**
 * @brief Inserts in BST in O(logn).
 * @param root BST root node
 * @param key key of the node to insert
 * @returns 0 when the operation succeeds, -1 otherwise (i.e. when the memory
 * needed for another node is not available or there exists a node in root
 * such that node.key == key).
*/
int bstInsert(tree_node_t** root, int key)
{
	if ( *root == NULL )
	{
		tree_node_t* leaf = (tree_node_t*) malloc(sizeof(tree_node_t));
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

/**
 * @brief Finds out whether there exists a node in root such that root.key == key.
 * @param root BST root
 * @param key key of the node to look for
 * @returns true if there exists a node in root such that root.key == key, false 
 * otherwise.
*/
bool bstFind(const tree_node_t* root, int key)
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

/**
 * @brief Prints out inorder visit of root.
 * @param root BST root
*/
void bstInorderVisit(const tree_node_t* root)
{
	if ( root != NULL )
	{
		bstInorderVisit(root->left);
		printf("%d ", root->key);
		bstInorderVisit(root->right);
	}
}

/**
 * @brief Deletes the node in BST such that node.key == key.
 * @param root BST root
 * @param key key of the node to be deleted
 * @returns new BST without the node which has its key equal to parameter key.
*/
tree_node_t* bstDeleteNode(tree_node_t* root, int key)
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
			tree_node_t* temp = root->right;
			free(root);
			return temp;
		}
		else if (root->right == NULL)
		{
			tree_node_t* temp = root->left;
			free(root);
			return temp;
		}
		tree_node_t* temp;
		MINIMUM_KEY(temp, root->left);
		root->key = temp->key;
		root->right = bstDeleteNode(root->right, temp->key);
	}
	return root;
}

/**
 * @brief Frees memory allocated for BST root.
 * @param root BST root
*/
void bstFree(tree_node_t* root)
{
	if ( root != NULL )
	{
		bstFree(root->left);
		bstFree(root->right);
		free(root);
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