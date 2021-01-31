#include <stdio.h>
#include <stdlib.h>

typedef struct _edges
{
    size_t num_edges;
    size_t *edges;
} edges;

typedef struct _queue {
    int *values; // elementi in coda
    int capacity; // capacità massima della coda
    int head; // posizione della testa
    int tail; // posizione della coda
    int size; // numero di elementi in coda
} queue;

// Inizializza una coda
void queue_init(queue *q, int capacity)
{
    q->capacity = capacity;
    q->values = (int*) malloc(sizeof(int) * capacity);
    q->head = 0;
    q->tail = -1;
    q->size = 0;
}

// Cancella una coda
void queue_deinit(queue *q)
{
    free(q->values);
}

// Mette in coda
void queue_pushBack(queue *q, int value)
{
    if ( q->size < q->capacity )
    {
        q->tail = (q->tail + 1) % q->capacity;
        q->values[q->tail] = value;
        q->size++;
    }
}

// Verifica che la coda sia vuota
int queue_is_empty(queue *q)
{
    if ( q->size == 0 )
        return 1;
    else return 0;
}

// Butta fuori il primo elemento della coda
int queue_popFront(queue *q)
{
    int primo = q->values[q->head];
    q->head = (q->head + 1) % q->capacity;
    q->size--; 
    return primo;
}

// Inizializza un grafo
edges* graph_init(size_t n)
{
    size_t i;
    edges *es = (edges*) malloc(sizeof(edges) * n);
    for ( i = 0; i < n; i++ )
        es[i].num_edges = 0;
    return es;
}

// Aggiunge un arco da from a to
void graph_add_edge(edges *graph, size_t from, size_t to)
{
    graph[from].edges[graph[from].num_edges] = to;
    graph[from].num_edges++;
}

// Funzione ausiliaria per visita dfs
void recursive_dfs(edges *graph, int src, int *colors) 
{
    int dest, i;
    for ( i = 0; i < graph[src].num_edges; i++ ) 
    {
        dest = graph[src].edges[i];
        if (!colors[dest])
        {
            colors[dest] = 1;
            printf("%d ", dest); // stampa la visita
            recursive_dfs(graph, dest, colors);
        }
    }
}

// Visita dfs
int *dfs(edges *graph, size_t n, size_t from) 
{
    size_t i;
    int *colors = (int *) malloc(sizeof(int)* (int) n);
    // inizializzo i colori
    for ( i = 0; i < n; i++ ) 
        colors[i] = 0;
    colors[from] = 1;
    // chiamata ricorsiva
    printf("%d ", from); // stampa la visita
    recursive_dfs(graph, from, colors);
    return colors;
}

// Visita bfs
int *bfs(edges *graph, size_t n, size_t from)
{
    int *distanza = (int*) malloc(sizeof(int) * n);
    queue q;
    size_t src, dest, i;
    // inizializzo i colori
    for (i = 0; i < n; i++) distanza[i] = -1;
    distanza[from] = 0;
    // inizializzo la coda
    queue_init(&q, n);
    queue_pushBack(&q, from);
    // loop fino a terminazione della coda
    while ( !queue_is_empty(&q) ) 
    {
        src = queue_popFront(&q);
        printf("%d ", src); // stampa la visita
        for ( i = 0; i < graph[src].num_edges; i++ ) 
        {
            dest = graph[src].edges[i];
            if ( distanza[dest] == -1 ) 
            {
                distanza[dest] = 1 + distanza[src];
                queue_pushBack(&q, dest);
            }
        }
    }
    // libero la memoria
    queue_deinit(&q);
    return distanza;
}

// Libera la memoria occupata dal grafo
void free_graph(edges *graph, size_t n)
{
    for ( size_t i = 0; i < n; i++ )
        free(graph[i].edges);
    free(graph);
}

int main()
{
    size_t n, from, to, ne, j;
    scanf("%lu", &n);
    edges *G = graph_init(n);
    for ( from = 0; from < n; from++ )
    {
        scanf("%lu", &ne);
        G[from].edges = (size_t*) malloc(sizeof(size_t) * ne);
        for ( j = 0; j < ne; j++ )
        {
            scanf("%lu", &to);
            graph_add_edge(G, from, to);
        }
    }
    // robe
    free_graph(G, n);
    return 0;
}