#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct _stringF
{
    char *text;
    int frequenza;
} stringF;

int confronta_stringhe(const void *a, const void *b)
{
    return strcmp( *(char**)a, *(char**)b );
}

int confronta_stringaF(const void *a, const void *b)
{
    return strcmp( ((stringF*)a)->text, ((stringF*)b)->text );
}

int confronta_freq(const void *a, const void *b)
{
    return ( ((stringF*)b)->frequenza - ((stringF*)a)->frequenza );
}

int main()
{
    stringF *arrstrF;
    char **arrChar;
    size_t n, k, i, j;
    scanf("%lu", &n);
    scanf("%lu", &k);
    arrstrF = (stringF*) malloc(sizeof(stringF) * n);
    arrChar = (char**) malloc(sizeof(char*) * n);
    for ( i = 0; i < n; i++ )
    {
        arrChar[i] = (char*) malloc(sizeof(char) * (101));
        scanf("%s", arrChar[i]);
    }
    j = 0;
    qsort(arrChar, n, sizeof(char*), confronta_stringhe);
    arrstrF[j].text = (char*) malloc(sizeof(char) * (101));
    strcpy(arrstrF[j].text, arrChar[0]);
    arrstrF[j].frequenza = 1;
    for ( i = 1; i < n; i++ )
    {
        if ( !strcmp(arrstrF[j].text, arrChar[i]) )
            arrstrF[j].frequenza++;
        else
        {
            j++;
            arrstrF[j].text = (char*) malloc(sizeof(char) * (101));
            strcpy(arrstrF[j].text, arrChar[i]);
            arrstrF[j].frequenza = 1;
        }
    }
    qsort(arrstrF, j+1, sizeof(stringF), confronta_freq);
    qsort(arrstrF, k, sizeof(stringF), confronta_stringaF);
    for ( i = 0; i < k; i++ )
    {
        printf("%s\n", arrstrF[i].text);
        free(arrstrF[i].text);
    }
    for ( i = 0; i < n; i++ )
        free(arrChar[i]);
}