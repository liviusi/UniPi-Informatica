#include <stdio.h>
#include <stdlib.h>

typedef struct _nodo
{
    int key;
    int left_visit;
    int right_visit;
    struct _nodo *left;
    struct _nodo *right;
} nodo;

void inserisci(nodo **t, int value)
{
    if ( *t == NULL )
    {
        nodo *foglia = (nodo*) malloc(sizeof(nodo));
        foglia->key = value;
        foglia->left_visit = 0;
        foglia->right_visit = 0;
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

void rec_visita(nodo *T, nodo *parent)
{
    if ( T->left != NULL )
        rec_visita(T->left, T);
    if ( T->right != NULL )
        rec_visita(T->right, T);
    
    if ( parent != NULL )
    {
        if ( T->key <= parent->key )
            parent->left_visit += T->left_visit + 1;
        if ( T->key > parent->key )
            parent->right_visit += T->right_visit + 1;
    }
}

void stampa_visita(nodo *T)
{
    if ( T != NULL )
    {
        stampa_visita(T->left);
        if ( T->left_visit > T->right_visit ) printf("%d\n", T->key);
        stampa_visita(T->right);
    }
}

void visita(nodo *T)
{
    nodo *parent = NULL;
    rec_visita(T, parent);
}

int main()
{
    int N, val;
    nodo *tree = NULL;
    scanf("%d", &N);
    if ( N <= 0 )
    {
        printf("%d", N);
        return 0;
    }
    for ( size_t i = 0; i < N; i++ )
    {
        scanf("%d", &val);
        inserisci(&tree, val);
    }
    visita(tree);
    stampa_visita(tree);
    svuota_albero(&tree);
}