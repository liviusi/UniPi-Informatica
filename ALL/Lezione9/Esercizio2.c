#include <stdio.h>
#include <stdlib.h>

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

int massimo(int a, int b)
{
    if ( a > b ) return a;
    else return b;
}

int altezza(nodo *t)
{
    if ( t == NULL ) 
        return 0;
    else 
        return massimo(altezza(t->right), altezza(t->left)) + 1;
}

void svuota_albero(nodo **t)
{
    if ( *t != NULL)
    {
        svuota_albero(&((*t)->right));
        svuota_albero(&((*t)->left));
        free(*t);
    }
}

int main()
{
    int N, i, val;
    nodo *abr = NULL;
    scanf("%d", &N);
    for ( i = 0; i < N; i++ )
    {
        scanf("%d", &val);
        inserisci(&abr, val);
    }
    printf("%d\n", altezza(abr));
    svuota_albero(&abr);
}