#include <stdio.h>
#include <stdlib.h>

typedef struct _node
{
    int key;
    struct _node *next;
} node;

// Crea una lista di un solo elemento
void create_list(node **lista, int val)
{
    *lista = (node*) malloc(sizeof(node));
    (*lista)->key = val;
    (*lista)->next = NULL; 
}

// Inserisce in testa in O(1)
void inserisci_in_testa(node **lista, int val)
{
    node *new_testa = (node*) malloc(sizeof(node));
    new_testa->key = val;
    new_testa->next = *lista;
    *lista = new_testa;
}

// Inserisce in coda in O(1)
node *inserisci_in_coda(node *ultimo, int val)
{
    ultimo->next = (node*) malloc(sizeof(node));
    ultimo->next->key = val;
    ultimo->next->next = NULL;
    return ultimo->next;
}

// Inserisce prima di un elemento in O(p) con p posizione
void inserisci_prima(node *lista, int toinsert, int flag)
{
    node *curr = lista;
    node *prev = NULL;
    if ( lista == NULL )
        inserisci_in_testa(&lista, toinsert);
    while ( curr != NULL && curr->key != flag )
    {
        prev = curr;
        curr = curr->next;
    }
    if ( curr == NULL ) inserisci_in_coda(prev, toinsert);
    else if ( curr->key == flag )
        inserisci_in_testa(curr, toinsert);    
}

// Inserisce dopo un elemento in O(p) con p posizione
void inserisci_dopo(node *lista, int toinsert, int flag)
{
    node *curr = lista;
    node *prev = NULL;
    if ( lista == NULL )
    {
        inserisci_in_testa(&lista, toinsert);
        return;
    }
    while ( curr != NULL && curr->key != flag )
    {
        prev = curr;
        curr = curr->next;
    }
    if ( curr == NULL ) 
    {
        inserisci_in_coda(prev, toinsert);
        return;
    }
    inserisci_in_testa(curr->next, toinsert);
    return;    
}

// Inserisci in ordine crescente
void inserisci_crescente(node *lista, int val)
{
    if ( lista == NULL )
    {
        inserisci_in_testa(&lista, val);
        return;
    }
    node *curr = lista;
    node *prev = NULL;
    while ( curr != NULL && curr->key < val )
    {
        prev = curr;
        curr = curr->next;
    }
    if ( prev == NULL ) 
    {
        inserisci_in_coda(&lista, val);
        return;
    }
    if ( curr == NULL )
    {
        inserisci_in_coda(&prev, val);
        return;
    }
    inserisci_in_testa(&curr, val);
}

// Inserisci in ordine decrescente
void inserisci_decrescente(node *lista, int val)
{
    if ( lista == NULL )
    {
        inserisci_in_testa(&lista, val);
        return;
    }
    node *curr = lista;
    node *prev = NULL;
    while ( curr != NULL && curr->key > val )
    {
        prev = curr;
        curr = curr->next;
    }
    if ( prev == NULL ) 
    {
        inserisci_in_coda(&lista, val);
        return;
    }
    if ( curr == NULL )
    {
        inserisci_in_coda(&prev, val);
        return;
    }
    inserisci_in_testa(&curr, val);
}

// Controlla se un elemento appartiene alla lista
int appartiene(node *lista, int val)
{
    if ( lista == NULL ) return 1;
    else
    {
        if ( lista->key != val ) return appartiene(lista->next, val);
        else return 0;
    }
    
}

// Stampa la lista
void stampa_lista(node *lista)
{
    while ( lista != NULL )
    {
        printf("%d -> ", lista->key);
        lista = lista->next;
    }
    printf("NULL");
}

// Libera la memoria allocata dalla lista
void svuota_lista(node **lista)
{
    while ( *lista != NULL )
    {
        node *dummy = *lista;
        (*lista) = (*lista)->next;
        free(dummy);
    }
}

/* int main()
{
    int val;
    node *lista = NULL;
    create_list(&lista, val);
    node *coda = lista;
    coda = inserisci_in_coda(coda, val);
    stampa_lista(lista);
    svuota_lista(&lista);
} */