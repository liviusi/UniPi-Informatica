#include <stdio.h>
#include <stdlib.h>
#define GET_MAX(x, y) ( (x) > (y) ? (x) : (y) )

int memoized_rod_cut(int *p, int n, int *r)
{
    if ( r[n] >= 0 ) return r[n];
    int q, i;

    if ( n == 0 ) return 0;
    else
    {
        for ( i = 0; i <= n; i++ )
            q = GET_MAX(q, p[i] + memoized_rod_cut(p, n - i, r));
    }
    r[n] = q;
    return q;
}

int *dynamic_rod_cut(int *p, int n, int *optimized_value)
{
    int *revenue = (int*) malloc(sizeof(int) * (n + 1));
    int *cutting = (int*) malloc(sizeof(int) * (n + 1));

    revenue[0] = 0;
    cutting[0] = 0;

    int i, j;
    
    for ( j = 1; j <= n; j++ )
    {
        int q = -1;
        for ( i = 1; i <= j; i++ )
            if ( q < p[i] + revenue[j - i] )
            {
                q = p[i] + revenue[j - i];
                cutting[j] = i;
            }
        revenue[j] = q;
    }

    *optimized_value = revenue[n];
    free(revenue);
    return cutting;
}

int main()
{
    int N;
    scanf("%d", &N);
    int *prezzi = (int*) malloc(sizeof(int) * (N + 1));
    prezzi[0] = 0;
    for ( int i = 1; i <= N; i++)
        scanf("%d", &(prezzi[i]));
    int ottimo = -1;
    int *cutting = dynamic_rod_cut(prezzi, N, &ottimo);
    printf("%d\n", ottimo);
    while ( N > 0 )
    {
        printf("%d ", cutting[N]);
        N -= cutting[N];
    }
    free(prezzi);
    free(cutting);
}