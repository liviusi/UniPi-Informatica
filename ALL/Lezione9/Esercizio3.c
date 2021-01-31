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

void sort_abr(nodo *t){
    if(t != NULL){
        sort_abr(t->left);
        printf("%d\n", t->key);
        sort_abr(t->right);
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
    sort_abr(abr);
    svuota_albero(&abr);
}