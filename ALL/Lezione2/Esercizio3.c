#include <stdio.h>
#define MAX_LENGTH 1000

int my_strlen(char *s)
{
    int i = 0;
    while (i < MAX_LENGTH && s[i] != '\0')
        i++;
    return i;
}

int main()
{
    char str[MAX_LENGTH];
    scanf("%s", str);
    printf("%d", my_strlen(str));
}