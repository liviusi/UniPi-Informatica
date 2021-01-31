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

void svuota_albero(nodo **t)
{
    if ( *t != NULL )
    {
        svuota_albero(&((*t)->left));
        svuota_albero(&((*t)->right));
        free(*t);
    }
}

int verificaCammino(nodo *t1, nodo *t2, int value)
{
    if ( t1 != NULL && t2 != NULL )
    {
        if ( t1->key == t2->key )
        {
            if ( t1->key == value )
                return 1;
            else 
                if ( value < t1->key )
                    return verificaCammino(t1->left, t2->left, value);
                else 
                    return verificaCammino(t1->right, t2->right, value);
        }
    }
    return 0;
}

int main()
{
    int i, N, K, val;
    nodo *abr1 = NULL, *abr2 = NULL;
    scanf("%d %d", &N, &K);
    for ( i = 0; i < N; i++ )
    {
        scanf("%d", &val);
        inserisci(&abr1, val);
    }
    for ( i = 0; i < N; i++ )
    {
        scanf("%d", &val);
        inserisci(&abr2, val);
    }
    printf("%d\n", verificaCammino(abr1, abr2, K));
    svuota_albero(&abr1);
    svuota_albero(&abr2);
}