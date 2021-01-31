#include <stdio.h>
#include <stdlib.h>

int intersect(int dimArray1, int dimArray2, 
        int *array1, int *array2)
{
    int i = 0, j = 0, val = 0;
    while ( i < dimArray1 && j < dimArray2 )
    {
        if ( array1[i] > array2[j])
            j++;
        else 
            if ( array1[i] < array2[j] )
                i++;
            else
            {
                val++;
                i++;
                j++;
            }
    }
    return val;
}

int main()
{
    int i, dim1, dim2;
    int *arr1, *arr2;
    scanf("%d", &dim1);
    arr1 = (int *) malloc(sizeof(int) * dim1);
    for ( i = 0; i < dim1;  i++ )
        scanf("%d", &(arr1[i]));
    scanf("%d", &dim2);
    arr2 = (int *) malloc(sizeof(int) * dim2);
    for ( i = 0; i < dim2; i++ )
        scanf("%d", (&arr2[i]));
    printf("%d", intersect(dim1, dim2, arr1, arr2));
    return 0;
}