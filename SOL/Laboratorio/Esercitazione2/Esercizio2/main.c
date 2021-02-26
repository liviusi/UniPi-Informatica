#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#define HELP "-n <num. intero> -m <num. intero> -o <stringa> -h\n"
#define SCONOSCIUTA "opzione non riconosciuta\n"
#define ERRORE_INTERO "richiede un parametro intero non opzionale.\n"
#define ERRORE_STRINGA "richiede un parametro di tipo stringa non opzionale.\n"

char is_digit(const char* str); // verifica che str contenga solo caratteri numerici

// handlers per i vari parametri
int arg_h(const char* str); // necessario per la sintassi del main
int arg_m(const char* str);
int arg_n(const char* str);
int arg_o(const char* str);

int main(int argc, char* argv[])
{
	// controllo di argc ed inizializzazione del vettore V con i puntatori a funzione
	if (argc < 2)
	{
		printf("usage: "HELP);
		return 1;
	}
	// Codici ASCII:
	// h mod 4 = 0
	// m mod 4 = 1
	// n mod 4 = 2
	// o mod 4 = 3
	int (*V[])(const char*) = { arg_h, arg_m, arg_n, arg_o }; // V è un vettore di puntatori a funzione
	int opt;
	opterr = 0; // necessario per la gestione manuale degli errori
	while ((opt = getopt(argc,argv, "n:m:o:h")) != -1)
	{
		switch(opt)
		{
			case '?':
			{
				if (optopt == 'n' || optopt == 'm')
					printf("-%c: "ERRORE_INTERO, optopt);
				else if (optopt == 'o')
					printf("-%c: "ERRORE_STRINGA, optopt);
				else printf("-%c: "SCONOSCIUTA, optopt);
				return 1;
			}
				break;
			default:
				// invocazione della funzione di gestione passando come parametro l'argomento restituito da getopt
				if (V[opt%4]( (optarg==NULL ? argv[0] : optarg) ) == -1)
					// <gestione errore>
					return 1;
		}
	}
	return 0;
}

char is_digit(const char* str)
{
	for(size_t i = 0; i < strlen(str); i++)
		if (str[i] < '0' || str[i] > '9') return 0;
	return 1;
}

int arg_h(const char* str)
{
	printf(HELP);
	return 0;
}

int arg_m(const char* str)
{
	if (is_digit(str))
	{
		printf("-m: %d\n", atoi(str));
		return 0;
	}
	printf("-m: "ERRORE_INTERO);
	return -1;
}

int arg_n(const char* str)
{
	if (is_digit(str))
	{
		printf("-n: %d\n", atoi(str));
		return 0;
	}
	printf("-n: "ERRORE_INTERO);
	return -1;
}

int arg_o(const char* str)
{
	if (str != NULL && str[0] != '-')
	{
		printf("-o: %s\n", str);
		return 0;
	}
	printf("-o: "ERRORE_STRINGA);
	return -1;
}