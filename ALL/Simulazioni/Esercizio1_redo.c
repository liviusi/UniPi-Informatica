#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct _nodo
{
    int key;
    struct _nodo *left;
    struct _nodo *right;
} nodo;

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

void svuota_albero(nodo **t)
{
    if ( *t != NULL )
    {
        svuota_albero(&((*t)->left));
        svuota_albero(&((*t)->right));
        free(*t);
    }
}

int sommamin(nodo *t, int *minimo)
{
    if ( t == NULL )
    {
        *minimo = INT_MIN;
        return 0;
    }
    else
    {
        int minsx, mindx;
        int sommasx = sommamin(t->left, &minsx);
        int sommadx = sommamin(t->right, &mindx);

        if ( sommasx > sommadx )
        {
            *minimo = minsx;
            return sommasx + t->key;
        }
        else if ( sommasx == sommadx )
        {
            if ( t->right == NULL && t->left == NULL )
                *minimo = t->key;
            else *minimo = minsx;
            return sommasx + t->key;
        }
        else
        {
            *minimo = t->key;
            return sommadx + t->key;
        }
    }
    
}

int main()
{
    int val, dim;
    scanf("%d", &dim);
    nodo *tree = NULL;
    if ( dim <= 0 )
    {
        printf("%d\n", dim);
        return 0;
    }
    for ( int i = 0; i < dim; i++ )
    {
        scanf("%d", &val);
        inserisci(&tree, val);
    }
    int minimo;
    int somma = sommamin(tree, &minimo);
    printf("Somma: %d\n", somma);
    printf("Minimo: %d\n", minimo);
    svuota_albero(&tree);
    return 0;
}