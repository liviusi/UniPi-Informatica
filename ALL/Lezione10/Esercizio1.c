#include <stdio.h>
#include <stdlib.h>
#define pr 999149

typedef struct _node {
 struct _node* next;
 int key;
} item;

int a, b, N;

int h(int x)
{
    return ((a*x + b) % pr) % (2*N);
}

int massimo(int a, int b)
{
    if ( a >= b ) return a;
    else return b;
}

void inserisci(item **t, int value)
{
    item *new_item = (item*) malloc(sizeof(item));
    new_item->next = NULL;
    new_item->key = value;
    if ( *t != NULL )
        inserisci(&((*t)->next), value);
    else *t = new_item;
}

/* void stampa_tabella(item *t)
{
    if ( t != NULL )
    {
        printf("%d -> ", t->key);
        stampa_tabella(t->next);
    }
} */

int lunghezza_massima(item *t)
{
    if ( t == NULL ) return 0;
    else return 1 + lunghezza_massima(t->next);
}

void svuota_tabella(item **t)
{
    item *dummy;
    while ( *t != NULL )
    {
        dummy = *t;
        *t = (*t)->next;
        free(dummy);
    }
}

int main()
{
    int i, val, cm = 0, cc = 0, c1, c2;
    item **table;
    scanf("%d", &N);
    scanf("%d", &a);
    scanf("%d", &b);
    table = (item **) malloc(2*N*sizeof(item *));
    for ( i = 0; i < 2*N; i++ ) 
        table[i] = NULL;
    for ( i = 0; i < N; i++ )
    {
        scanf("%d", &val);
        inserisci(&table[ h(val) ], val);
    }
/*      for ( i = 0; i < 2*N; i++ )
    {
        printf("%d: ", i);
        stampa_tabella(table[i]);
        printf("NULL \n");
    } */
    for ( i = 0; i < 2*N - 1; i+=2 )
    {
        c1 = lunghezza_massima(table[i]);
        c2 = lunghezza_massima(table[i+1]);
        // printf("Lunghezza %d: %d\n", i, c1);
        // printf("Lunghezza %d: %d\n", (i + 1), c2);
        if ( c1 - 1 > 0 ) cc += (c1 - 1);
        if ( c2 - 1 > 0 ) cc += (c2 - 1);
        // printf("Aumentata di: %d\n", cc);
        cm = 1 + massimo(c1, c2);
    }
    printf("%d\n%d", cm, cc);
    svuota_tabella(table);
    return 0;
}