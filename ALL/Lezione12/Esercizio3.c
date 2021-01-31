#include <stdio.h>
#include <stdlib.h>

typedef struct _edges
{
    size_t num_edges;
    size_t *edges;
} edges;

typedef struct _queue {
    int *values;
    int capacity;
    int head;
    int tail;
    int size;
} queue;

void queue_init(queue *q, int capacity)
{
    q->capacity = capacity;
    q->values = (int*) malloc(sizeof(int) * capacity);
    q->head = 0;
    q->tail = -1;
    q->size = 0;
}

void queue_deinit(queue *q)
{
    free(q->values);
}

void queue_pushBack(queue *q, int value)
{
    if ( q->size < q->capacity )
    {
        q->tail = (q->tail + 1) % q->capacity;
        q->values[q->tail] = value;
        q->size++;
    }
}

int queue_is_empty(queue *q)
{
    if ( q->size == 0 )
        return 1;
    else return 0;
}

int queue_popFront(queue *q)
{
    int primo = q->values[q->head];
    q->head = (q->head + 1) % q->capacity;
    q->size--; 
    return primo;
}

edges *graph_init(size_t n)
{
    size_t i;
    edges *es = (edges*) malloc(sizeof(edges) * n);
    for ( i = 0; i < n; i++ )
        es[i].num_edges = 0;
    return es;
}

void graph_add_edge(edges *graph, size_t from, size_t to)
{
    graph[from].edges[graph[from].num_edges] = to;
    graph[from].num_edges++;
}

void free_graph(edges *graph, size_t n)
{
    size_t i;
    for ( i = 0; i < n; i++ )
        free(graph[i].edges);
    free(graph);
}

int shortest_path(edges *graph, size_t n, size_t from, size_t to)
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
        for ( i = 0; i < graph[src].num_edges; i++ ) 
        {
            dest = graph[src].edges[i];
            if ( distanza[dest] == -1 ) 
            {
                distanza[dest] = 1 + distanza[src];
                queue_pushBack(&q, dest);
                if ( dest == to )
                {
                    queue_deinit(&q);
                    return distanza[to];
                }
            }
        }
    }
    // libero la memoria
    queue_deinit(&q);
    return distanza[to];
}

int main()
{
    size_t i, j, n, ne, queries, from, to;
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
    scanf("%lu", &queries);
    for ( i = 0; i < queries; i++ )
    {
        scanf("%lu %lu", &from, &to);
        printf("%d\n", shortest_path(G, n, from, to));
    }
    free_graph(G, n);
}