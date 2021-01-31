#include <stdio.h>
#include <stdlib.h>

typedef struct _edges
{
    size_t num_edges;
    size_t *edges;
} edges;

edges* graph_init(size_t n)
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

void recursive_dfs(edges *graph, int src, int *colors) 
{
    int dest, i;
    for ( i = 0; i < graph[src].num_edges; i++ ) 
    {
        dest = graph[src].edges[i];
        if (!colors[dest])
        {
            colors[dest] = 1;
            recursive_dfs(graph, dest, colors);
        }
    }
}

int *dfs(edges *graph, size_t n, size_t from) 
{
    size_t i;
    int *colors = (int *) malloc(sizeof(int)* (int) n);
    // inizializzo i colori
    for ( i = 0; i < n; i++ ) 
        colors[i] = 0;
    colors[from] = 1;
    // chiamata ricorsiva
    recursive_dfs(graph, from, colors);
    return colors;
}

int graph_is_connected(int *colors, size_t n)
{
    size_t i;
    for ( i = 0; i < n; i++ )
        if ( colors[i] == 0 ) return 0;
    return 1;
}

void free_graph(edges *graph, size_t n)
{
    size_t i;
    for ( i = 0; i < n; i++ )
        free(graph[i].edges);
    free(graph);
}

int main()
{
    size_t n, from, ne, i, j;
    int *colors;
    scanf("%lu", &n);
    edges *G = graph_init(n);
    for ( from = 0; from < n; from++ )
    {
        scanf("%lu", &ne);
        G[from].edges = (size_t*) malloc(sizeof(size_t) * ne);
        for ( j = 0; j < ne; j++ )
        {
            scanf("%lu", &i);
            graph_add_edge(G, from, i);
        }
    }
    colors = dfs(G, n, 0);
    printf("%d\n", graph_is_connected(colors, n));
    free(colors);
    free_graph(G, n);
    return 0;
}