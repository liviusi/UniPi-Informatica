#include <stdio.h>
#include <stdlib.h>

void swap(int* xp, int* yp) 
{ 
    if (xp == yp) // Check if the two addresses are same 
        return; 
    *xp = *xp + *yp; 
    *yp = *xp - *yp; 
    *xp = *xp - *yp;
}

void InsertionSort(int *array, int dim)
{
    int j, i = 1;
    while ( i < dim )
    {
        j = i;
        while ( j > 0 && array[ j - 1 ] > array[j] )
        {
            swap(&(array[j]), &(array[ j - 1 ]));
            j--;
        }
        i++;
    }
}

int main()
{
    int dim, i;
    int *arr;
    scanf("%d", &dim);
    arr = (int*) malloc(sizeof(int) * dim);
    for ( i = 0; i < dim; i++ )
        scanf("%d", &(arr[i]));
    InsertionSort(arr, dim);
    for ( i = 0; i < dim; i++ )
        printf("%d\n", arr[i]);
    free(arr);
}