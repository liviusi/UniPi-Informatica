#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_LENGTH 1000

char* recproduct(char *inputstr, int stillToCall, int startingPoint, char *outputstr)
{
    if (stillToCall > 0)
    {
        int i = 0;
        while (inputstr[i] != '\0')
        {
            outputstr[startingPoint + i] = inputstr[i];
            i++;
        }
        recproduct(inputstr, stillToCall - 1, startingPoint + strlen(inputstr), outputstr);
    }
    else 
    {
        outputstr[startingPoint] = '\0';
        return outputstr;
    }
}

char* product(char *str, int k)
{
    char* productstring = (char*) malloc((sizeof(char) * strlen(str) * k) + 1);
    return recproduct(str, k, 0, productstring);
}

int main()
{
    int k;
    char s1[MAX_LENGTH + 1];
    scanf("%s", s1);
    scanf("%d", &k);
    printf("%s", product(s1, k));
    return 0;
}