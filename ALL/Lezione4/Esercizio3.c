#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LENGTH 100

typedef char* string;

int legge(string **strs, int *dim)
{
    int i;
    scanf("%d", dim);
    if ( dim <= 0 ) return 1;
    if ( (*strs) == NULL ) return 1;
    (*strs) = (string*) malloc((*dim) * sizeof(string));
    for ( i = 0; i < *dim; i++ )
    {
        (*strs)[i] = (string) malloc(sizeof(char) * (MAX_LENGTH + 1));
        scanf("%s", (*strs)[i]);
    }
    return 0;
}

void InsertionSort(string *strs, int dim)
{
    int i, j;
    string value;
    for ( i = 1; i < dim; i++ )
    {
        value = strs[i];
        j = i - 1;
        while ( j >= 0 && strcmp(value, strs[j]) <= 0 )
        {
            strs[ j + 1 ] = strs[j];
            j--;
        }
        strs[ j + 1 ] = value;
    }
}

int main()
{
    int dim, i;
    string* arr;
    legge(&arr, &dim);
    /* 
    for ( i = 0; i < dim; i++ )
        printf("Stringa %d: %s\n", i, arr[i]); */
    InsertionSort(arr, dim);
    for ( i = 0; i < dim; i++ )
        {
            printf("%s\n", arr[i]);
            free(arr[i]);
        }
    free(arr);
}