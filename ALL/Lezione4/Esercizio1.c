#include <stdio.h>
#include <stdlib.h>

void SelectionSort(int *array, int dim)
{
    int i, j, dummy, posmin;
    for ( i = 0; i < dim; i++)
    {
        posmin = i;
        for ( j = i + 1; j < dim; j++ )
            if ( array[j] < array[posmin] )
                posmin = j;
        if ( posmin != i )
        {
            dummy = array[i];
            array[i] = array[posmin];
            array[posmin] = dummy;
        }
    }

}

int main()
{
    int i, dim;
    int *arr;
    scanf("%d", &dim);
    arr = (int*) malloc(sizeof(int) * dim);
    for ( i = 0; i < dim; i++ )
        scanf("%d", &(arr[i]));
    SelectionSort(arr, dim);
    for ( i = 0; i < dim; i++ )
    {
        printf("%d\n", arr[i]);
    }
    free(arr);
}