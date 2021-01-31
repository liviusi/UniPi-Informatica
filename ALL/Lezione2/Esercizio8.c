#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LENGTH 1000

int min(int val1, int val2)
{
    if (val1 >= val2) return val2;
    else return val1;
}

void my_strcmp(char *s1, char *s2)
{
    int i = 0;
    int val = min(strlen(s1), strlen(s2));
    while ( val >= i )
        if ( s1[i] > s2[i] )
        { 
            printf("+1");
            return;
        }
        else if ( s1[i] < s2[i] ) 
        {
            printf("-1");
            return;
        }
            else i++;
    printf("0");
}


int main()
{
    char str1[MAX_LENGTH + 1], str2[MAX_LENGTH +1];
    scanf("%s", str1);
    scanf("%s", str2);
    my_strcmp(str1, str2);
}