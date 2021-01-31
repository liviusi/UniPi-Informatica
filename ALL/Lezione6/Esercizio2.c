#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LENGTH 100

typedef char* string;

int legge(string **strs, int *dim)
{
    int i;
    scanf("%d", dim);
    if ( (*dim) <= 0 ) return 1;
    (*strs) = (string*) malloc((*dim) * sizeof(string));
    if ( (*strs) == NULL ) return 1;
    for ( i = 0; i < (*dim); i++ )
    {
        (*strs)[i] = (string) malloc((MAX_LENGTH + 1) * sizeof(char));
        scanf("%s", (*strs)[i]);
    }
    return 0;
}

int confrontaStr(const void *a, const void *b)
{
    return - (strcmp( *(string*)a, *(string*)b ));
}

int main()
{
    int i, dim;
    string *arr;
    if ( legge(&arr, &dim) ) return 1;
    qsort(arr, dim, sizeof(string), confrontaStr);
    for ( i = 0; i < dim; i++ )
    {
        printf("%s\n", arr[i]);
        free(arr[i]);
    }
    free(arr);
}