#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAX_LENGTH 100

typedef char* string;

int RicercaBinaria(string *strs, int dim, string target)
{
    int m;
    int l = 0, r = dim - 1;
    while ( l <= r )
    {
        m = (int) floor((l + r) / 2);
        if ( strcmp(strs[m], target) < 0 )
            l = m + 1;
        else if ( strcmp(strs[m], target) > 0 )
            r = m - 1;
        else return m;
    }
    return -1;
}

int legge(string **strs, int *dim)
{
    int i;
    scanf("%d", dim);
    if ( (*dim) <= 0 ) return 1;
    if ( (*strs) == NULL ) return 1;
    (*strs) = (string*) malloc((*dim) * sizeof(string));
    for ( i = 0; i < (*dim); i++ )
    {
        (*strs)[i] = (string) malloc((MAX_LENGTH + 1 ) * sizeof(string));
        scanf("%s", (*strs)[i]);
    }
    return 0;
}

int main()
{
    int dim, i;
    string *arr, checkstr;
    if ( legge(&arr, &dim) ) return 1;
    /* 
    for ( i = 0; i < dim; i++ )
        printf("Stringa %d: %s\n", i, arr[i]); */
    i = scanf("%d", &i);
    while (i)
    {
        checkstr = (string) malloc(sizeof(string) * (MAX_LENGTH + 1 ));
        scanf("%s", checkstr);
        printf("%d\n", RicercaBinaria(arr, dim, checkstr));
        free(checkstr);
        scanf("%d", &i);
    }
    for ( i = 0; i < dim; i++)
        free(arr[i]);
    free(arr);
}