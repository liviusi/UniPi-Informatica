#include <stdio.h>
#define MAX_LENGTH 1000

int anagramma(unsigned char* s1, unsigned char* s2)
{
    // Instantiating array of zeroes
    int dummys1[256] = { 0 };
    int dummys2[256] = { 0 };
    int i = 0;
    int stillTrue = 1;
    while (i < MAX_LENGTH && (s1[i] != '\0' && s2[i] != '\0'))
    {
        dummys1[ (int) (s1[i]) ]++;
        dummys2[ (int) (s2[i]) ]++;
        i++;
    }
    i = 0;
    while (i < 256 && stillTrue)
    {
        if (dummys1[i] != dummys2[i])
            stillTrue = 0;
        i++;
    }
    return stillTrue;
}

int main()
{
    char str1[MAX_LENGTH], str2[MAX_LENGTH];
    scanf("%s", str1);
    scanf("%s", str2);
    printf("%d", anagramma(str1, str2));
}