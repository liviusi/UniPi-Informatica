#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_LENGTH 1000

void my_strcpy(char* src, char* dest)
{
    int i;
    for (i = 0; i <= strlen(src); i++)
        dest[i] = src[i];
    dest[i] = '\0';
}

int main()
{
    int length;
    char s1[MAX_LENGTH + 1];
    char *s2;
    scanf("%s", s1);
    s2 = (char*) malloc(sizeof(char) * (int) strlen(s1));
    my_strcpy(s1, s2);
    printf("%s", s2);
    return 0;
}