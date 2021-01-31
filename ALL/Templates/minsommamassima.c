#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define MINIMO(x, y) ( ( x <= y ) ? ( x ) : ( y ) )

typedef struct _node
{
    int key;
    struct _node *left;
    struct _node *right;
} node;

struct result
{
    int min;
    int sum;
};

void inserisci(node **tree, int val)
{
    if ( !(*tree) )
    {
        node *foglia = (node*) malloc(sizeof(node));
        foglia->key = val;
        foglia->left = NULL;
        foglia->right = NULL;
        *tree = foglia;
    }
    else if ( val <= (*tree)->key )
        inserisci(&((*tree)->left), val);
    else inserisci(&((*tree)->right), val);
}

void svuota_albero(node **tree)
{
    if ( *tree )
    {
        svuota_albero(&((*tree)->left));
        svuota_albero(&((*tree)->right));
        free(*tree);
    }
}

node *readABR(size_t dim)
{
    size_t i;
    int k;
    node *tree = NULL;
    for ( i = 0; i < dim; i++ )
    {
        scanf("%d", &k);
        inserisci(&tree, k);
    }
    return tree;
}

struct result auxminsomma(const node *tree, int min, int somma)
{
    if ( !tree )
    {
        struct result res = { min, somma };
        return res;
    }
    else
    {
        struct result ressx, resdx;
        ressx = ( tree->left ) ? ( auxminsomma(tree->left, MINIMO(min,tree->left->key), somma 
            + tree->left->key)) : ( auxminsomma(tree->left, min, somma) );
        resdx = ( tree->right ) ? ( auxminsomma(tree->right, min, somma 
            + tree->right->key)) : ( auxminsomma(tree->right, min, somma) );
        return ( resdx.sum > ressx.sum ) ? ( resdx ) : ( ressx );
    }
    
}

int minsommamassima(const node *tree)
{
    return auxminsomma(tree, tree->key, 0).min;
}

int main()
{
    size_t dim;
    scanf("%lu", &dim);
    if ( dim == 0 )
    {
        printf("0\n");
        return 0;
    }
    node *tree = readABR(dim);
    printf("%d\n", minsommamassima(tree));
    return 0;
}