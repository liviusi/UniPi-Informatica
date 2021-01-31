#include <stdio.h>
#include <stdlib.h>

/* Elemento di una lista doppiamente linkata:
 * Contiene l'informazione, un puntatore all'elemento successivo
 * e un puntatore all'elemento precedente della lista.
 */
typedef struct _elemento_di_lista {
    int val; // Informazione
    int count; // Contatore del numero di volte che questo elemento è stato cercato
    struct _elemento_di_lista* next; // Puntatore al prossimo elemento
    struct _elemento_di_lista* prev; // Puntatore all'elemento precedente
} list_elem_t;

/*
 * Inserimento in coda in una lista doppiamente linkata.
 * In questo caso basta ricevere il puntatore alla coda
 * e collegarci il nuovo elemento, sistemando anche il puntatore al predecessore.
 *
 * Come procedimento è simile ad un inserimento in testa in una lista monodirezionale.
 *
 * La coda della lista deve essere passata con la & dal main, perché cambia ogni volta.
 * Serve anche un puntatore alla testa, perché in caso di lista vuota
 * anche la testa cambia.
 */
void inserisci_in_coda(list_elem_t** tail, list_elem_t** head, int elem) {
    /* Creazione del nuovo elemento */
    list_elem_t* new = malloc(sizeof(list_elem_t));
    new->val = elem;
    new->count = 0;
    new->next = NULL; // Il nuovo elemento non ha un successore
    new->prev = *tail; // L'elemento precedente al nuovo è la vecchia coda
    
    if(*tail != NULL) {
        (*tail)->next = new;
    }
    
    *tail = new;
    
    if(*head == NULL) {
        *head = new;
    }
}

int search_and_count(list_elem_t** head, list_elem_t** tail, int i) {
    list_elem_t* curr = *head;
    int posizione = 0;
    
    while(curr != NULL && curr->val != i) {
        curr = curr->next;
        posizione++;
    }
    
    /* Se curr è diventato NULL vuol dire che i non era in lista. */
    if(curr == NULL) {
        return -1;
    }
    
    (curr->count)++;
    
    while(curr->prev != NULL && curr->count > curr->prev->count) {
        /* Dobbiamo scambiare curr e curr->prev.
         * Per prima cosa sistemiamo il predecessore (curr->prev)
         */
        list_elem_t* vecchio_predecessore = curr->prev;
        
        /* Il nostro nuovo predecessore è il precedente del vecchio */
        curr->prev = vecchio_predecessore->prev;
        /* Il nostro vecchio predecessore ora punta al nostro vecchio successore */
        vecchio_predecessore->next = curr->next;
        /* Adesso siamo noi il predecessore del nostro vecchio predecessore */
        vecchio_predecessore->prev = curr;
        
        /* Sistemiamo il nostro nuovo successore (curr->next), se c'è */
        if(curr->next != NULL)
            curr->next->prev = vecchio_predecessore; // Il suo predecessore è il nostro vecchio
        
        /* Il nostro nuovo predecessore deve puntare a noi */
        if(curr->prev != NULL)
            curr->prev->next = curr;
        
        /* Infine il nostro successore è chi prima ci precedeva */
        curr->next = vecchio_predecessore;
    }
    
    if(curr->prev == NULL) {
        /* Siamo finiti in testa, dobbiamo modificarne il puntatore */
        *head = curr;
    }
    
    if(curr->next == NULL) {
        /* Siamo finiti in coda, dobbiamo modificarne il puntatore */
        *tail = curr;
    }
    
    return posizione;
    
}

int main(void) {
    int i;
    int n;
    
    scanf("%i", &n);
    
    /* Manteniamo un puntatore sia alla testa, per scorrere la lista,
     * che alla coda, per inserirci un nuovo elemento.
     * NB: devono essere entrambi NULL all'inizio.
     */
    list_elem_t* tail = NULL;
    list_elem_t* head = NULL;
    
    for(i=0;i<n;i++) {
        int tmp;
        scanf("%i", &tmp);
        /* Inserimento degli elementi */
        inserisci_in_coda(&tail, &head, tmp);
    }
    
    int result;
    do {
        int tmp;
        scanf("%i", &tmp);
        /* Passiamo sia head che tail perché potremmo dover modificare entrambi */
        result = search_and_count(&head, &tail, tmp);
        printf("%i\n", result);
    }
    while(result != -1);
    
    /* Deallocazione della memoria (classica) */
    while(head != NULL) {
        list_elem_t* tmp = head;
        head = head->next;
        free(tmp);
    }
    
    return 0;
}