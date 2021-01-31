#include <stdio.h>
#include <stdlib.h>

typedef struct _nodo
{
    int key;
    struct _nodo *next;
} nodo;

void inserisci(nodo **head, int value)
{
    if ( *head != NULL )
        inserisci( &((*head)->next), value );
    else
    {
        nodo *nuovo = (nodo*) malloc(sizeof(nodo));
        nuovo->key = value;
        nuovo->next = NULL;
        *head = nuovo;
    }
}

void stampa_lista(nodo *head)
{
    if ( head != NULL )
    {
        printf("%d ", head->key);
        stampa_lista(head->next);
    }
}

int sommesuffisse(nodo* head)
{
    int somma;
    if ( head == NULL )
        return 0;
    somma = head->key;
    head->key = sommesuffisse(head->next);
    return somma + head->key;
}

void svuota_lista(nodo **head)
{
    nodo *dummy;
    if ( *head != NULL )
    {
        dummy = *head;
        svuota_lista( &((*head)->next) );
        free(dummy);
    }
}

int main()
{
    int i, N, val;
    nodo *lista = NULL;
    scanf("%d", &N);
    for ( i = 0; i < N; i++ )
    {
        scanf("%d", &val);
        inserisci(&lista, val);
    }
    stampa_lista(lista);
    printf("\n");
    sommesuffisse(lista);
    stampa_lista(lista);
    svuota_lista(&lista);
}