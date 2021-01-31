#include <stdio.h>
#include <stdlib.h>
#define MAX_LENGTH 1000

void my_strcat(char* s1, char* s2)
{
    int j = 0;
    int i = 0;
    while (s1[i] != '\0')
        i++;
    while (s2[j] != '\0')
    {
        s1[i] = s2[j];
        i++;
        j++;
    }
}


int main()
{
    int i = 0;
    char str1[MAX_LENGTH], str2[MAX_LENGTH];
    scanf("%s %s", str1, str2);
    my_strcat(str1, str2);
    while (i < MAX_LENGTH && str1[i] != '\0')
    {
        printf("%c", str1[i]);
        i++;
    }
}