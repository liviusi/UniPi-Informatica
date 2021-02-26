#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#define HELP "-n <num. intero> -m <num. intero> -o <stringa> -h\n"
#define ERRORE "sintassi non corretta\n"
#define SCONOSCIUTA "opzione non riconosciuta\n"

char is_digit(char* str); // verifica che str contenga solo caratteri numerici

int main(int argc, char* argv[])
{
	if (argc == 1)
	{
		printf("usage: "HELP);
		return 1;
	}
	int opt;
	opterr = 0; // necessario per la gestione manuale degli errori
	int n, m; // numeri interi passati come parametro ai flag n, m
	char* o; // stringa passata come parametro al flag o
	while ((opt = getopt(argc,argv, "n:m:o:h")) != -1)
	{
		switch(opt)
		{
			case 'n':
				if (is_digit(optarg))
				{
					n = atoi(optarg);
					printf("-n: %d\n", n);
				}
				else printf("-n: "ERRORE);
				break;
			case 'm':
				if (is_digit(optarg))
				{
					m = atoi(optarg);
					printf("-m: %d\n", m);
				}
				else printf("-m: "ERRORE);
				break;
			case 'o':
				o = optarg;
				if (o[0] != '-')
					printf("-o: %s\n", o);
				else printf("-o: "ERRORE);
				break;
			case 'h':
				printf(HELP);
				break;
			default:
				if (optopt != 'n' && optopt != 'm' && optopt != 'o')
					printf("-%c: "SCONOSCIUTA, optopt);
				else printf("-%c: "ERRORE, optopt);
				return 1;
		}
	}
	return 0;
}

char is_digit(char* str)
{
	for(size_t i = 0; i < strlen(str); i++)
		if (str[i] < '0' || str[i] > '9') return 0;
	return 1;
}