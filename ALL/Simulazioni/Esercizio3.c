#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LENGTH 101
#define MAX_OGGETTI 15

typedef struct _nodo
{
    int key;
    char *text;
    struct _nodo *next;
} item;

typedef struct _oggetti
{
    int key;
    char *text;
} oggetti;

int N; // variabili globali in C?

int h(char *stringa)
{
    size_t i = 0;
    int somma = 0;
    while ( i < strlen(stringa) )
    {
        somma += (unsigned int) stringa[i];
        i++;
    }
    return somma % (2 * N);
}

void inserisci_in_testa(item **t, char *string, int value)
{
    item *nuovo = (item*) malloc(sizeof(item));
    nuovo->text = (char*) malloc(sizeof(char) * MAX_LENGTH);
    strcpy(nuovo->text, string);
    nuovo->key = value;
    nuovo->next = *t;
    *t = nuovo;
}

void inserisci(item **t, char *string, int value)
{
    if ( *t == NULL ) // se la lista è vuota
    {
        inserisci_in_testa(t, string, value); // aggiunto un elemento
    }
    else
    {
        item *dummy = *t; // variabile di ciclo
        while ( dummy != NULL )
        {
            if ( strcmp(dummy->text, string) == 0 ) // l'oggetto è gia presente
            {
                if ( dummy->key >= value ) // se l'affetto è maggiore, esci
                    return; // aggiunti 0 elementi
                else 
                {
                    dummy->key = value; // rimpiazza con l'affetto maggiore
                    return; // aggiunti 0 elementi
                }
            }
            dummy = dummy->next;
        }
        // se il loop termina e non si è inserito nulla
        inserisci_in_testa(t, string, value); // aggiunto un elemento
    }
}

/* void stampa_tabella(item *t)
{
    if ( t != NULL )
    {
        printf("%d %s -> ", t->key, t->text);
        stampa_tabella(t->next);
    }
} */

void svuota_tabella(item **t)
{
    item *dummy;
    while ( *t != NULL )
    {
        dummy = *t;
        *t = (*t)->next;
        free(dummy->text);
        free(dummy);
    }
}

oggetti *hash_to_array(item **t, size_t *distinti, int N) // da jagged array a array 1d
{
    item *walk = NULL;
    size_t i;
    size_t j = 0;
    oggetti *arr = (oggetti*) malloc(sizeof(oggetti) * N);
    for (i = 0; i < (2 * N); i++ )
    {
        walk = t[i];
        while ( walk != NULL )
        {
            arr[j].text = (char*) malloc(sizeof(char) * MAX_LENGTH);
            strcpy(arr[j].text, walk->text);
            arr[j].key = walk->key;
            j++;
            walk = walk->next;
        }
    }
    *distinti = j;
    return arr;
}

int cmpkey(const void *a, const void *b)
{
    oggetti o1 = *(oggetti*)a;
    oggetti o2 = *(oggetti*)b;
    int diff = o2.key - o1.key; // ordine decrescente
    if ( diff == 0 )
        return strcmp(o1.text, o2.text);
    return diff;
}

void svuota_array(oggetti *arr, int dim)
{
    for ( size_t i = 0; i < dim; i++ )
        free(arr[i].text);
    free(arr);
}

int main()
{
    size_t i;
    int value;
    scanf("%d", &N); // leggi N da input
    char *string = (char*) malloc(sizeof(char) * MAX_LENGTH);
    item **table = (item**) malloc(sizeof(item*) * (2*N));
    for ( i = 0; i < 2 * N; i++ )
        table[i] = NULL;
    for ( i = 0; i < N; i++ )
    {
        scanf("%s", string);
        scanf("%d", &value);
        inserisci( (&table[h(string)]), string, value );
    }
/*     for ( i = 0; i < 2 * N; i++ )
    {
        printf("%lu: ", i);
        stampa_tabella(table[i]);
        printf("NULL\n");
    } */
    i = 0;
    size_t dis = 0;
    oggetti *array = hash_to_array(table, &dis, N);
    qsort(array, dis, sizeof(oggetti), cmpkey);
    while ( i < dis && i < MAX_OGGETTI )
    {
        // printf("%lu: %d ", i, array[i].key);
        printf("%s\n", array[i].text);
        i++;
    }
    free(string);
    svuota_array(array, dis);
    svuota_tabella(table);
}