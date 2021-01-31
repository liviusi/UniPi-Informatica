#include <stdio.h>
#include <stdlib.h>

typedef struct _node
{
    int key;
    struct _node *left;
    struct _node *right;
} node;

void inserisci(node **tree, int value)
{
    if ( !(*tree) )
    {
        node *foglia = (node*) malloc(sizeof(node));
        foglia->key = value;
        foglia->left = NULL;
        foglia->right = NULL;
        *tree = foglia;
    }
    else
        if (value <= (*tree)->key )
            inserisci(&((*tree)->left), value);
        else
            inserisci(&((*tree)->right), value);
}

int sbilanciamento(node *tree, int *out)
{
    if ( tree )
    {
        int dx, sx;
        dx = sbilanciamento(tree->right, out);
        sx = sbilanciamento(tree->left, out);
        *out = ( abs(dx - sx) > *out) ? (abs(dx - sx)) : (*out);
        return 1 + dx + sx;
    }
    else return 0;
}

void svuota_albero(node **t)
{
    if ( !(*t) )
    {
        svuota_albero(&((*t)->left));
        svuota_albero(&((*t)->right));
        free(*t);
    }
}

int main()
{
    node *tree = NULL;
    size_t dim, i;
    int n;
    scanf("%lu", &dim);
    if ( dim == 0 )
        return 0;
    for ( i = 0; i < dim; i++ )
    {
        scanf("%d", &n);
        inserisci(&tree, n);
    }
    int sbil = 0;
    int ok = (dim == sbilanciamento(tree, &sbil)) ? (1) : (0);
    if ( ok ) printf("%d", sbil);
    return 0;
}