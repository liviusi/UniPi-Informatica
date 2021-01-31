#include <stdio.h>
#include <stdlib.h>
#define pr 999149

typedef struct _nodo
{
    int key;
    struct _nodo *next;
} item;

int N;
int a;
int b;

int h(int x)
{
    return ((a*x + b) % pr) % (2*N);
}

int inserisci_distinti(item **t, int value)
{
    if ( *t == NULL )
    {
        item *nuovo = (item*) malloc(sizeof(item));
        nuovo->key = value;
        nuovo->next = NULL;
        *t = nuovo;
        return 1;
    }
    else 
        if ( (*t)->key != value )
            return inserisci_distinti(&((*t)->next), value);
        else
            return 0;
}

/* void stampa_tabella(item *t)
{
    if ( t != NULL )
    {
        printf("%d -> ", t->key);
        stampa_tabella(t->next);
    }
}
 */
int lunghezza_conflitti(item *t)
{
    if ( t == NULL ) return 0;
    else return 1 + lunghezza_conflitti(t->next);
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

int massimo(int a, int b)
{
    if ( a >= b ) return a;
    else return b;
}

int main()
{
    int i, val, c1, c2, cm = 0, cc = 0, distinti = 0;
    item **table;
    scanf("%d", &N);
    scanf("%d", &a);
    scanf("%d", &b);
    table = (item**) malloc(sizeof(item*) * (2 * N));
    for ( i = 0; i < 2 * N; i++ )
        table[i] = NULL;
    for ( i = 0; i < N; i++ )
    {
        scanf("%d", &val);
        if ( inserisci_distinti(&table[ h(val) ], val) ) distinti++;
    }
/*     for ( i = 0; i < 2*N; i++ )
    {
        printf("%d: ", i);
        stampa_tabella(table[i]);
        printf("NULL \n");
    } */
    for ( i = 0; i < 2*N - 1; i+=2 )
    {
        c1 = lunghezza_conflitti(table[i]);
        c2 = lunghezza_conflitti(table[i+1]);
        // printf("Lunghezza %d: %d\n", i, c1);
        // printf("Lunghezza %d: %d\n", (i + 1), c2);
        if ( c1 - 1 > 0 ) cc += (c1 - 1);
        if ( c2 - 1 > 0 ) cc += (c2 - 1);
        cm = massimo(cm, c1);
        cm = massimo(cm, c2);
    }
    printf("%d\n%d", cc, cm);
    printf("\n%d", distinti);
    svuota_tabella(table);
}