#include <stdio.h>
#include <stdlib.h>

int recintersect(int val, int startingPoint, int dimArray1, 
            int dimArray2, int *array1, int *array2)
{
    if ( startingPoint < dimArray1 )
    {
        int i = 0;
        while ( i < dimArray2 )
            if ( array1[startingPoint] == array2[i] )
            {
                i++;
                val++;
            }
            else i++;
        recintersect(val, startingPoint + 1, dimArray1, 
                dimArray2, array1, array2);
    }
    else return val;
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
    printf("%d", recintersect(0, 0, dim1, dim2, arr1, arr2));
    return 0;
}