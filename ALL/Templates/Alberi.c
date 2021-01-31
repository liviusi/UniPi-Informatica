#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct _nodo
{
    int key;
    struct _nodo *left;
    struct _nodo *right;
} nodo;

// Inserisce in ABR
void inserisci(nodo **t, int value)
{
    if ( *t == NULL )
    {
        nodo *foglia = (nodo*) malloc(sizeof(nodo));
        foglia->key = value;
        foglia->left = NULL;
        foglia->right = NULL;
        *t = foglia;
    }
    else
    {
        if ( value <= (*t)->key )
            inserisci(&((*t)->left), value);
        else
            inserisci(&((*t)->right), value);
    }
}

// Cerca in un ABR
int trova_elemento(nodo *t, int val)
{
    if ( t == NULL ) return 0;
    else
    {
        if ( t->key > val ) return trova_elemento(t->left, val);
        else 
            if ( t->key == val ) return 1;
            else return trova_elemento(t->right, val); 
    }
    
}

// Leggi un ABR da input
nodo *leggiABR(size_t dim)
{
    nodo *tree = NULL;
    int key;
    for ( size_t i = 0; i < dim; i++ )
    {
        scanf("%d", &key);
        inserisci(&tree, key);
    }
    return tree;
}

// Visita simmetrica
void inorder_visit(nodo *t)
{
    if ( t != NULL )
    {
        inorder_visit(t->left);
        printf("%d ", t->key);
        inorder_visit(t->right);
    }
}

// Libera la memoria occupata dall'ABR
void svuota_albero(nodo **t)
{
    if ( *t != NULL )
    {
        svuota_albero(&((*t)->left));
        svuota_albero(&((*t)->right));
        free(*t);
    }
}

int main()
{
    size_t dim;
    scanf("%lu", &dim);
    if ( dim == 0 ) return 0;
    nodo *tree = leggiABR(dim);
    // robe
    svuota_albero(&tree);
    return 0;
}