#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define P 999149

// Struct elemento di lista
typedef struct node_t
{
    int key;
    struct node_t *next;
} node;

// Struct tabella hash
typedef struct table_t
{
    node **bins;
    size_t size;
    int a, b;
} hashTable;

// Inizializza una lista
node *newList()
{
    return NULL;
}

// Inserisce in testa
void listInsert(node **head, int k)
{
    node *retVal = (node*) malloc(sizeof(node));
    retVal->key = k;
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
    free(list);
    destroyList(succ);
}

// Funzione hash
int hash(int x, int a, int b, size_t size)
{
    return ((a*x) + b % P) % size;
}

// Inizializza una tabella hash di dimensione size
hashTable *newHashTable(size_t size, int a, int b)
{
    hashTable *retVal = (hashTable*)malloc(sizeof(hashTable));
    retVal->size = size;
    retVal->a = a;
    retVal->b = b;
    retVal->bins = (node**) malloc(sizeof(node*) * size);
    int i;
    for ( i = 0; i < size; i++ )
        retVal->bins[i] = newList();
    return retVal;
}

// Inserisce val nella posizione corretta, gestisce i conflitti con liste di trabocco
// monodirezionali. Inserisce solo elementi non presenti.
void hashInsert(hashTable *t, int val)
{
    int index = hash(val, t->a, t->b, t->size);
    if ( listSearch(t->bins[index], val) == 0 ) // Non presente
        listInsert(&(t->bins[index]), val);
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

// Restituisce alcune statistiche di t
void hashStats(hashTable * t)
{
    int conflitti=0, maxlen = 0, numel= 0;
    for ( size_t i = 0; i < t->size; i++ )
    {
        int len = listLength(t->bins[i]);
        if (len > maxlen)
            maxlen = len;
        numel += len;
        if (len > 1)
            conflitti += len -1;
    }
    printf("%d\n%d\n%d\n", conflitti, maxlen, numel);
}

// Stampa la lista list
void print_list(node *list)
{
    if ( list != NULL )
    {
        printf("%d -> ", list->key);
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

int main()
{
    // srand(time(NULL))
    // (rand() % 9999) + 1
    int a, b;
    size_t dim;
    scanf("%lu", &dim);
    scanf("%d", &a);
    scanf("%d", &b);
    hashTable * table = newHashTable(2*dim, a, b);
    int val;
    for ( size_t i = 0; i < dim; i++ ) {
        scanf("%d", &val);
        hashInsert(table, val);
    }
    print_hash(table);
    hashStats(table);
    destroyHash(table);
    return 0;
}