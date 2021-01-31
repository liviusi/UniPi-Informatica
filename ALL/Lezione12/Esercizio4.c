#include <stdio.h>
#include <stdlib.h>
#define GET_MAX(x, y) ((x) > (y) ? (x) : (y))

typedef struct _edges
{
    size_t num_edges;
    size_t *edges;
} edges;

typedef struct _queue
{
    size_t capacity;
    size_t size;
    size_t *values;
    size_t head;
    size_t tail;
} queue;

queue *queue_init(queue *q, size_t N)
{
    q->capacity = N;
    q->values = (size_t*) malloc(sizeof(size_t) * N);
    q->head = 0;
    q->tail = -1;
    q->size = 0;
    return q;
}

size_t queue_is_empty(queue *q)
{
    return q->size == 0;
}

void enqueue(queue *q, size_t val)
{
    if ( q->size < q->capacity )
    {
        q->tail = (q->tail + 1) % q->capacity;
        q->values[q->tail] = val;
        q->size++;
    }
}

size_t dequeue(queue *q)
{
    size_t first = q->values[q->head];
    q->head = ( q->head + 1 ) % q->capacity;
    q->size--;
    return first;
}

void queue_deinit(queue *q)
{
    free(q->values);
}

edges *graph_init(size_t N)
{
    size_t i;
    edges *es = (edges*) malloc(sizeof(edges) * N);
    for ( i = 0; i < N; i++ )
        es[i].num_edges = 0;
    return es;
}

void graph_add_edge(edges *graph, size_t from, size_t to)
{
    graph[from].edges[graph[from].num_edges] = to;
    graph[from].num_edges++;
}

void graph_deinit(edges *graph, size_t N)
{
    size_t i;
    for ( i = 0; i < N; i++ )
        free(graph[i].edges);
    free(graph);
}

int* shortest_path(edges *graph, size_t n, size_t from)
{
    int *distanza = (int*) malloc(sizeof(int) * n);
    queue q;
    size_t src, dest, i;
    // inizializzo i colori
    for (i = 0; i < n; i++) 
    {
        distanza[i] = -1;
    }
    distanza[from] = 0;
    // inizializzo la coda
    queue_init(&q, n);
    enqueue(&q, from);
    // loop fino a terminazione della coda
    while ( !queue_is_empty(&q) ) 
    {
        src = dequeue(&q);
        for ( i = 0; i < graph[src].num_edges; i++ ) 
        {
            dest = graph[src].edges[i];
            if ( distanza[dest] == -1 ) 
            {
                distanza[dest] = 1 + distanza[src];
                enqueue(&q, dest);
            }
        }
    }
    // libero la memoria
    queue_deinit(&q);
    return distanza;
}

int diametro_grafo(edges *graph, int N)
{
    size_t j;
    size_t i = 0;
    int d = 0;
    int *array_distanze;
    while ( i < N )
    {
        array_distanze = shortest_path(graph, N, i);
        j = 0;
        while ( j < N )
        {
            if ( array_distanze[j] == -1 )
            {
                free(array_distanze);
                return -1;
            }
            else 
            {
                d = GET_MAX(d, array_distanze[j]);
                j++;
            }
        }
        i++;
    }
    free(array_distanze);
    return d;
}

int main()
{
    size_t N, i, from, to, ne;
    scanf("%lu", &N);
    edges *G = graph_init(N);
    for ( from = 0; from < N; from++ )
    {
        scanf("%lu", &ne);
        G[from].edges = (size_t*) malloc(sizeof(size_t) * ne);
        for ( i = 0; i < ne; i++ )
        {
            scanf("%lu", &to);
            graph_add_edge(G, from, to);
        }
    }
    printf("%d\n", diametro_grafo(G, N));
    graph_deinit(G, N);
}