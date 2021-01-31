#include <stdio.h>

void reset(int array[], int len)
{
    int i;
    for (i = 0; i < len; i++)
        array[i] = 0;
}

void add(int array[], int len, int val)
{
    if (len - val)
        array[val]++;
}

int main()
{
    int arr[10];
    int len = 10, n = 0;
    reset(arr, len);
    while (n+1)
    {
        scanf("%d", &n);
        add(arr, len, n);
    }
    for (n = 0; n < len-1; n++)
        printf("%d\n", arr[n]);
}