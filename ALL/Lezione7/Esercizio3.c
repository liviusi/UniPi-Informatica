#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _string 
{
    char text[21];
    char anagramma[21];
} string;

int confronta_anagramma(const void* s1, const void* s2)
{
    int res = strcmp( ((string*) s1)->anagramma, ((string*) s2)->anagramma );
    if ( res == 0 ) // se anagramma
        return strcmp( ((string*) s1)->text, ((string*) s2)->text );
    else return res;
}

int confronta_stringhe(const void* c1, const void* c2){
    return *((char*) c1) - *((char*) c2);
}

int main(){
    int dim, i, j;
    string* arr;
    scanf("%d", &dim);
    arr = (string*) malloc(dim*sizeof(string));
    j = 0;
    for ( i = 0; i < dim; i++ )
    {
        scanf("%s", arr[i].text);
        strcpy(arr[i].anagramma, arr[i].text);
        qsort(arr[i].anagramma, strlen(arr[i].anagramma), sizeof(char), confronta_stringhe);
    }
    qsort(arr, dim, sizeof(string), confronta_anagramma);
    for( i = 0; i < dim; i++ )
    {
        if( j && strcmp( arr[i].anagramma, arr[i-1].anagramma ) != 0 )
            printf("\n");
        printf("%s ", arr[i].text);
        j = 1;
    }
    free(arr);
}