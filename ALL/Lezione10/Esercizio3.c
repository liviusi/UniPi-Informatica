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
        nodo *nuovo_nodo = (nodo*) malloc(sizeof(nodo));
        nuovo_nodo->key = value;
        nuovo_nodo->next = NULL;
        *head = nuovo_nodo;
    }
}

int calcola_media(nodo *head, int elementi)
{
    int somma = 0;
    while ( head != NULL )
    {
        somma += head->key;
        head = head->next;
    }
    return (int) ( (float) (somma / elementi));
}

void cancella(nodo **head, int media)
{
    nodo *dummy;
    if ( (*head) != NULL )
    {
        if ( (*head)->key <= media )
        {
            dummy = *head;
            *head = (*head)->next;
            free(dummy);
            cancella(head, media);
        }
        else
            cancella(&((*head)->next), media);
    }
}

void stampa_lista(nodo *head)
{
    while ( head != NULL )
    {
        printf("%d ", head->key);
        head = head->next;
    }
    printf("\n");
}

void svuota_lista(nodo **head)
{
    nodo *dummy;
    if ( *head != NULL )
    {
        svuota_lista(&((*head)->next));
        dummy = *head;
        free(dummy);
    }
}

int main()
{
    int i, N, media, value;
    nodo *lista = NULL;
    scanf("%d", &N);
    for ( i = 0; i < N; i++ )
    {
        scanf("%d", &value);
        inserisci(&lista, value);
    }
    media = calcola_media(lista, N);
    printf("%d\n", media);
    stampa_lista(lista);
    cancella(&lista, media);
    stampa_lista(lista);
    return 0;
}