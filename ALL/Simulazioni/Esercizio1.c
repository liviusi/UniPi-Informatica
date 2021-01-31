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

int _massimo(int a, int b)
{
    if ( a >= b ) return a;
    else return b;
}

int _minimo(int a, int b)
{
    if ( a > b ) return b;
    else return a;
}

int sommamax(nodo *t, int *min)
{
    if ( t == NULL )
    {
        *min = 0;
        return 0;
    }
    else
    {
        int minsx, mindx;
        int sommasx = sommamax(t->left, &minsx);
        int sommadx = sommamax(t->right, &mindx);

        if ( sommasx > sommadx )
        {
            *min = minsx;
            return sommasx + t->key;
        }
        else if ( sommasx == sommadx )

        {
            if ( t->left == NULL && t->right == NULL )
                *min = t->key;
            // else *min = ( minsx < mindx ) ? minsx : mindx;
            else *min = minsx;
            return sommadx + t->key;
        }
        else
        {
            *min = t->key;
            return sommadx + t->key;
        }
    }
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
    int minimo;
    int somma = sommamax(tree, &minimo);
    // printf("%d\n", somma);
    printf("%d", minimo);
    svuota_albero(&tree);
    return 0;
}