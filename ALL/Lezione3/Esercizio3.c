#include <stdio.h>
#include <stdlib.h>

int maxSottoarray(int dim, int *arr)
{
    int max = 0, somma = 0, i;
    for ( i = 0; i < dim; i++ )
    {
        if ( somma > 0 )
            somma += arr[i];
        else 
            somma = arr[i];
        if ( somma > max ) max = somma;
    }
    return max;
}

int main()
{
    int *arr;
    int dim;
    int i = 0;
    scanf("%d", &dim);
    arr = ( int* ) malloc(sizeof(int) * dim);
    while ( i < dim )
    {
        scanf("%d", &(arr[i]));
        i++;
    }
    printf("%d", maxSottoarray(dim, arr));
    return 0;
}