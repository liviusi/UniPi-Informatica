#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_NOME 101
#define MAX_NUMERO 21

typedef struct _item
{
    char nome[MAX_NOME];
    char numero[MAX_NUMERO];
    struct _item *next;
} item;

// Funzione hash, prende nome e grandezza tabella hash
int hash(char *string, size_t modulo)
{
    unsigned somma = 0;
    size_t i = 0;
    while ( i < strlen(string) )
    {
        somma += (unsigned) string[i];
        i++;
    }
    return (int) somma % modulo;
}

// Inserisce un elemento nella lista di trabocco alla posizione corretta
void inserisci(item **hashtable, char *nome, char *numero)
{
    if ( *hashtable != NULL )
    {
        if ( strcmp(nome, (*hashtable)->nome) < 0 )
        {
            item *newnode = (item*) malloc(sizeof(item));
            strcpy(newnode->nome, nome);
            strcpy(newnode->numero, numero);
            newnode->next = *hashtable;
            *hashtable = newnode;
        }
        else
            inserisci( &((*hashtable)->next), nome, numero );
        
    }
    else
    {
        *hashtable = (item*) malloc(sizeof(item));
        strcpy((*hashtable)->nome, nome);
        strcpy((*hashtable)->numero, numero);
        (*hashtable)->next = NULL;
    }
    
}

// Funzione ausiliaria per svuotare la lista di trabocco
void svuota_lista(item *lista)
{
    item *dummy;
    while ( lista != NULL )
    {
        dummy = lista;
        lista = lista->next;
        free(dummy);
    }
}

// Libera la memoria occupata dalla tabella hash
void svuota_tabella(item **hashtable, size_t dimensione)
{
    for ( size_t i = 0; i < dimensione; i++ )
        svuota_lista(hashtable[i]);
    free(hashtable);
}

// Stampa la lista di trabocco in una posizione data
void stampa_lista(item *lista)
{
    if ( lista != NULL )
    {
        printf("%s %s\n", lista->nome, lista->numero);
        stampa_lista(lista->next);
    }
}

// Stampa interamente la tabella hash
/*
void stampa_tabella(item **hashtable, size_t dimensione)
{
    for ( size_t i = 0; i < dimensione; i++ )
    {
        printf("%lu: ", i);
        stampa_lista(hashtable[i]);
        printf("NULL\n");
    }
} */

int main()
{
    size_t N;
    char nome[MAX_NOME];
    char numero[MAX_NUMERO];
    scanf("%lu", &N);
    item **table = (item**) malloc(sizeof(item*) * (2 * N));
    for ( size_t i = 0; i < (2*N); i++ )
        table[i] = NULL;
    for ( size_t i = 0; i < N; i++ )
    {
        scanf("%s", nome);
        scanf("%s", numero);
        inserisci( &(table[hash(nome, 2*N)]), nome, numero );
    }
    int K;
    scanf("%d", &K);
    stampa_lista(table[K]);
    svuota_tabella(table, 2*N);
}

/*
4
Gianni
050-123456
Carlo
050-234567
Maria
050-345678
Antonella
050-456789
6 
*/
