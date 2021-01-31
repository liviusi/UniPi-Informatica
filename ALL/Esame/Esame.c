#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define pr 109
#define MAX_LENGTH 101

// Struct elemento di lista
typedef struct node_t
{
    int key;
    char *str;
    struct node_t *next;
} node;

// Struct tabella hash
typedef struct table_t
{
    node **bins;
    size_t size;
} hashTable;

// Inizializza una lista
node *newList()
{
    return NULL;
}

// Inserisce in testa
void listInsert(node **head, int k, char *str)
{
    node *retVal = (node*) malloc(sizeof(node));
    retVal->key = k;
    retVal->str = (char*) malloc(sizeof(char) * MAX_LENGTH);
    strcpy(retVal->str, str);
    retVal->next = *head;
    *head = retVal;
}

// Controlla se val ricorre in head
int listSearch(node *head, int val)
{
    node *curr = head;
    for ( ; curr != NULL; curr = curr->next )
        if ( curr->key == val )
            return 1;
    return 0;
}

// Restituisce la lunghezza di list
int listLength(node *list)
{
    if ( !list )
        return 0;
    return 1 + listLength(list->next);
}

// Libera la memoria allocata da list
void destroyList(node* list)
{
    if ( !list ) // l == NULL
        return;

    node *succ = list->next;
    free(list->str);
    free(list);
    destroyList(succ);
}

// Funzione hash
int hash(int x, size_t size)
{
    // h(x) = (x % 109) % 2N
    return ( x % pr ) % size;
}

// Inizializza una tabella hash di dimensione size
hashTable *newHashTable(size_t size)
{
    hashTable *retVal = (hashTable*)malloc(sizeof(hashTable));
    retVal->size = size;
    retVal->bins = (node**) malloc(sizeof(node*) * size);
    int i;
    for ( i = 0; i < size; i++ )
        retVal->bins[i] = newList();
    return retVal;
}

// Inserisce val nella posizione corretta, gestisce i conflitti con liste di trabocco
// monodirezionali. Inserisce solo elementi non presenti.
void hashInsert(hashTable *t, int val, char *str)
{
    int index = hash(val, t->size);
    if ( listSearch(t->bins[index], val) == 0 ) // Non presente
        listInsert(&(t->bins[index]), val, str);
    else strcpy(t->bins[index]->str, str); // Cambia stringa
}

// Libera la memoria allocata da t
void destroyHash(hashTable *t)
{
    int i;
    for ( i = 0; i < t->size; i++ )
        destroyList(t->bins[i]);
    free(t->bins);
    free(t);
}

// Stampa la lista list
void print_list(node *list)
{
    if ( list != NULL )
    {
        printf("%d ", list->key);
        printf("%s -> ", list->str);
        print_list(list->next);
    }
}

// Stampa interamente la tabella hash
void print_hash(hashTable *table)
{
    for ( size_t i = 0; i < table->size; i++ )
    {
        printf("%lu: ", i);
        print_list(table->bins[i]);
        printf("NULL\n");
    }
}

int cmpstr (const void *p1, const void *p2) 
{
    return strcmp( *(char **)p1, *(char **)p2 );
}

void lista_to_array(node *head, int dim)
{
    int dis = 0;
    char **arr = (char**) malloc(sizeof(char*) * dim);
    for ( size_t i = 0; head && i < dim; i++ )
    {
        arr[i] = head->str;
        // arr[i] = (char*) malloc(sizeof(char) * MAX_LENGTH);
        // strcpy(arr[i], head->str);
        dis++;
        head = head->next;
    }
    qsort(arr, dis, sizeof(char*), cmpstr);
    for ( size_t i = 0; i < dis; i++ )
        printf("%s\n", arr[i]);
    return;
}

void stampa_ordinata(hashTable *table, int pos)
{
    if ( pos >= table->size )
    {
        printf("vuota\n");
        return;
    }
    else
    {
        int index = hash(pos, table->size);
        size_t dim = table->size / 2;
        if ( table->bins[index] )
        {
            lista_to_array(table->bins[index], dim);
            return;
        }
        else
        {
            printf("vuota\n");
            return;
        }
    }
    
}

int main()
{
    size_t dim;
    scanf("%lu", &dim);
    hashTable *table = newHashTable(2*dim);
    int val;
    char str[MAX_LENGTH];
    for ( size_t i = 0; i < dim; i++ ) 
    {
        scanf("%d", &val);
        scanf("%s", str);
        hashInsert(table, val, str);
    }
    int K;
    scanf("%d", &K);
    stampa_ordinata(table, K);
    // print_hash(table);
    destroyHash(table);
    return 0;
}