#include <stdio.h>
#include <stdlib.h>

typedef struct _elem
{
    int key;
    struct _elem *next;
} elem;

void crea_lista(elem **lista, int n)
{
    *lista = (elem*) malloc(sizeof(elem));
    (*lista)->key = n;
    (*lista)->next = NULL;
}

elem *aggiungi_in_coda(elem *ultimo_elemento, int n)
{
    ultimo_elemento->next = (elem*) malloc(sizeof(elem));
    ultimo_elemento->next->key = n;
    ultimo_elemento->next->next = NULL;
    return ultimo_elemento->next;
}

/* void stampa_lista(elem *lista)
{
    while ( lista != NULL )
    {
        printf("%d -> ", lista->key);
        lista = lista->next;
    }
    printf("NULL\n");
} */

void svuota_lista(elem **lista)
{
    elem *dummy;
    while ( *lista != NULL )
    {
        dummy = *lista;
        *lista = (*lista)->next;
        free(dummy);
    }
}

int move_to_front(elem **lista, int n)
{
    elem *prev = NULL;
    elem *curr = *lista;
    int posizione = 0;
    if ( (*lista)->key == n ) return posizione;
    while ( curr != NULL && curr->key != n )
    {
        prev = curr;
        curr = curr->next;
        posizione++;
    }
    if ( curr == NULL )
        return -1;
    else
    {
        elem *dummy = *lista;
        prev->next = curr->next;
        *lista = curr;
        curr->next = dummy;
        return posizione;
    }
}

int main()
{
    int N, i, inputval, outputval;
    elem *lista = NULL;
    elem *coda = NULL;
    scanf("%d", &N);
    scanf("%d", &inputval);
    crea_lista(&lista, inputval);
    coda = lista;
    for ( i = 1; i < N; i++ )
    {
        scanf("%d", &inputval);
        coda = aggiungi_in_coda(coda, inputval);
    }
    while ( outputval != -1 )
    {
        scanf("%d", &inputval);
        outputval = move_to_front(&lista, inputval);
        printf("%d\n", outputval);
    }
    svuota_lista(&lista);
}