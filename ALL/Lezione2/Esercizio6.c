#include <stdio.h>
#include <stdlib.h>

char* my_strcat2(int length1, char* str1, int length2, char* str2)
{
    int j = 0, i = 0;
    char* cat = malloc(sizeof(char) * (length1 + length2 + 1));
    do
    {
        cat[i] = str1[i];
        i++;
    } while (str1[i] != '\0');
    do
    {
        cat[i + j] = str2[j];
        j++;
    } while (str2[j] != '\0');
    cat[i + j + 1] = '\0';
    return cat;
}

int main()
{
    int l1, l2;
    char *s1, *s2;
    int i = 0;
    scanf("%d", &l1);
    s1 = (char*) malloc(sizeof(char) * (l1 + 1));
    scanf("%s", s1);
    scanf("%d", &l2);
    s2 = (char*) malloc(sizeof(char) * (l2 + 1));
    scanf("%s", s2);
    printf("%s", my_strcat2(l1, s1, l2, s2));
    return 0;
}