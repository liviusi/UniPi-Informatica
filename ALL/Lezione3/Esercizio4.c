#include <stdio.h>
#include <stdlib.h>

int legge(int **arr, int *dim)
{
    int i;
    scanf("%d", dim);
    if ( (*dim) <= 0 ) return 1;
    (*arr) = (int *) malloc((*dim) * sizeof(int));
    if ( (*arr) == NULL) return 1;
    for ( i = 0; i < *dim; i++ )
    {
        scanf("%d", (*arr)+i);
    }
    return 0;
}

int *unioneOrdinata(int *array1, int dim1, int *array2, int dim2)
{
    int i = 0, k = 0, j = 0, dimUnione = dim1 + dim2;
    int *outputArray = (int*) malloc(sizeof(int) * dimUnione);
    for ( ; k < dimUnione; k++ )
    {
        if ( i >= dim1 ) outputArray[k] = array2[j++];
        else if ( j >= dim2 ) outputArray[k] = array1[i++];
        else if ( array1[i] < array2[j] ) outputArray[k] = array1[i++];
        else outputArray[k] = array2[j++];
    }
    return outputArray;
}

int main()
{
    int dim1, dim2, i, *arr1, *arr2, *unionArr;
    if ( legge(&arr1, &dim1) ) return 1;
    if ( legge(&arr2, &dim2) ) return 1;
    /* for ( i = 0; i < dim1; i++ )
        printf("arr1, elemento %d: %d\n", i, arr1[i]);
    for ( i = 0; i < dim2; i++ )
        printf("arr2, elemento %d: %d\n", i, arr2[i]); */
    unionArr = unioneOrdinata(arr1, dim1, arr2, dim2);
    for ( i = 0; i < (dim1 + dim2); i++ )
        printf("%d\n", unionArr[i]);
    free(arr1);
    free(arr2);
    free(unionArr);
    return 0;
}