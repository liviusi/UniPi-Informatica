#include <tokenizer.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// esempio di tokenizzazione di stringhe con strtok_r

void tokenizer_r(char *stringa)
{
	char *tmpstr;
	char *token = strtok_r(stringa, " ", &tmpstr);
	while (token)
	{
		printf("%s\n", token);
		token = strtok_r(NULL, " ", &tmpstr);
	}
}

// esempio di tokenizzazione di stringhe con strtok (non rientrante!)

void tokenizer(char *stringa)
{
	char* token = strtok(stringa, " ");
	while (token)
	{
		printf("%s\n", token);
		token = strtok(NULL, " ");
	}
}