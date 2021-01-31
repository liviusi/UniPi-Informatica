#include <stdio.h>
#include <stdlib.h>

int legge(int **array, int *dim)
{
    int i;
    scanf("%d", dim);
    if ( (*dim) <= 0 ) return 1;
    (*array) = (int*) malloc((*dim) * sizeof(int));
    if ( (*array) == NULL ) return 1;
    for ( i = 0; i < (*dim); i++ )
        scanf("%d", (*array)+i);
    return 0;
}

int confrontaInt(const void *a, const void *b)
{
    if ( !(*(int*)a & 1) ) // se a pari
    {
        if ( !(*(int*)b & 1) ) // se b pari
            return *(int*)a - *(int*)b;
        else // b dispari
            return -1;
    }
    else // a dispari
    {
        if ( !(*(int*)b & 1) ) // se b pari
            return 1;
        else // b dispari
            return -(*(int*)a - *(int*)b);
    }
}

int main()
{
    int i, dim, *arr;
    if ( legge(&arr, &dim) ) return 1;
    qsort(arr, dim, sizeof(int), confrontaInt);
    for ( i = 0; i < dim; i++ )
        printf("%d\n", arr[i]);
    free(arr);
}