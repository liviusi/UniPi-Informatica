#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LENGTH 100

typedef struct _stringL
{
    char *string;
    int length;
} stringL;

int legge(stringL **strs, int *dim)
{
    int i;
    scanf("%d", dim);
    if ( (*dim) <= 0 ) return 1;
    (*strs) = (stringL*) malloc((*dim) * sizeof(stringL));
    if ( (*strs) == NULL ) return 1;
    for ( i = 0; i < (*dim); i++ )
    {
        (*strs)[i].string = (char*) malloc(sizeof(char) * (MAX_LENGTH + 1));
        scanf("%s", (*strs)[i].string);
        (*strs)[i].length = strlen((*strs)[i].string);
    }
    return 0;
}

int confrontaStringL(const void *a, const void *b)
{
    stringL *s1 = (stringL*) a;
    stringL *s2 = (stringL*) b;
    if ( s1->length != s2->length ) return ( s1->length - s2->length );
    else return strcmp(s1->string, s2->string); 
}

int main()
{
    int dim, i;
    stringL *arr;
    if ( legge(&arr, &dim) ) return 1;
    qsort(arr, dim, sizeof(stringL), confrontaStringL);
    for ( i = 0; i < dim; i++ )
    {
        printf("%s\n", arr[i].string);
        free(arr[i].string);
    }
    free(arr);
    return 0;
}