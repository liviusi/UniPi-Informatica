#include <stdlib.h>
#include <stdio.h>

typedef struct _nodo {
 int key;
 struct _nodo *left;
 struct _nodo *right;
} nodo;

int cercaRic(nodo* t, int key) {
    if ( t == NULL ) return -1;
    if ( key == t->key ) return 0;
    int found = -1;
    if ( key > t->key )
        found = cercaRic(t->right,key);
    else
        found = cercaRic(t->left,key);
    if ( found >= 0 ) 
        return 1 + found;
    else 
        return -1;
} 

void inserisci(nodo **t, int value){
    if( *t == NULL )
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
        else inserisci(&((*t)->right), value);
    }
}


void svuota_albero(nodo **abr)
{
    if ( *abr != NULL )
    {
        svuota_albero(&((*abr)->left));
        svuota_albero(&((*abr)->right));
        free(*abr);
    } 
}

int main()
{
    int i, N, value, outval;
    nodo *abr = NULL;
    scanf("%d", &N);
    for ( i = 0; i < N; i++ )
    {
        scanf("%d", &value);
        inserisci(&abr, value);
    }
    do
    {
        scanf("%d", &i);
        if ( i >= 0 )
        {
            outval = cercaRic(abr, i);
            if ( outval != -1 )
                printf("%d\n", outval);
            else 
                printf("NO\n"); 
        }
    } while ( i >= 0 );
    svuota_albero(&abr);
}