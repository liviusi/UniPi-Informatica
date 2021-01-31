#include <stdio.h>
#include <stdlib.h>

typedef struct _threenodo
{
    int key;
    struct _threenodo *left;
    struct _threenodo *center;
    struct _threenodo *right;
} threenodo;

void inserisci(threenodo **t, int value)
{
    if ( *t == NULL )
    {
        threenodo *foglia = (threenodo*) malloc(sizeof(threenodo));
        foglia->key = value;
        foglia->left = NULL;
        foglia->center = NULL;
        foglia->right = NULL;
        *t = foglia;
    }
    else
    {
        if ( value < (*t)->key )
            inserisci(&((*t)->left), value);
        else 
            if ( (value % (*t)->key) == 0 )
                inserisci(&((*t)->center), value);
            else inserisci(&((*t)->right), value);
    }
}

void svuota_albero(threenodo **t)
{
    if ( *t != NULL )
    {
        svuota_albero(&((*t)->left));
        svuota_albero(&((*t)->center));
        svuota_albero(&((*t)->right));
        free(*t);
    }
}

int tre_figli(threenodo *t)
{
    int outval = 0;
    if ( t == NULL ) return 0;
    else
    {
        if ( t->left != NULL && t->center != NULL && t->right != NULL )
            outval = 1;
    }
    return outval + tre_figli(t->left) + tre_figli(t->center) + tre_figli(t->right);
}

int main()
{
    int N, i, val;
    threenodo *atr = NULL;
    scanf("%d", &N);
    for ( i = 0; i < N; i++ )
    {
        scanf("%d", &val);
        inserisci(&atr, val);
    }
    printf("%d\n", tre_figli(atr));
    svuota_albero(&atr);
}