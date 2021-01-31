#include <stdio.h>
#include <stdlib.h>

typedef struct _point
{
    int x;
    int y;
} point;

int legge(point **array, int *dim)
{
    int i;
    scanf("%d", dim);
    if ( (*dim) <= 0 ) return 1;
    (*array) = (point*) malloc((*dim) * sizeof(point));
    if ( (*array) == NULL ) return 1;
    for ( i = 0; i < (*dim); i++ )
        scanf("%d %d", &(((*array)+i)->x), &(((*array)+i)->y));
    return 0;
}

int confrontaPunto(const void *a, const void *b)
{
   point* p1 = (point*) a;
   point* p2 = (point*) b;
   if ( p1->x != p2->x ) return p1->x - p2->x;
   else return - ( p1->y - p2->y );
}

int main()
{
    int dim, i;
    point *arr;
    if ( legge(&arr, &dim) ) return 1;
    qsort(arr, dim, sizeof(point), confrontaPunto);
    for ( i = 0; i < dim; i++ )
        printf("%d %d\n", arr[i].x, arr[i].y);
    free(arr);
    return 0;
}