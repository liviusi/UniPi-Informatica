#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define NOMEPROGRAMMA "cmdlineparsing"
#define HELP "nome-programma -n <numero> -s <stringa> -m <numero> -h\n"
#define SCONOSCIUTA "e' una opzione non riconosciuta\n"
#define ERRORE "messaggio di errore\n"

bool n_flag = false, m_flag = false, s_flag = false, h_flag = false, unknown_flag = false;
size_t n_index = 0, m_index = 0, s_index = 0, unknown_index = 0;

long isNumber(const char* s);

void aggiorna_flags(const char* str, size_t index);

void parsing_cmd(int argc, char* argv[]);

int main(int argc, char* argv[])
{
	if ( argc < 2 ) return 1;
	// if ( strcmp(argv[1], NOMEPROGRAMMA ) != 0) return 1;
	for ( size_t i = 1; i < argc; i++ )
		aggiorna_flags(argv[i], i);
	parsing_cmd(argc, argv);
	return 0;
}

long isNumber(const char* s)
{
	char* e = NULL;
	long val = strtol(s, &e, 0);
	if (e != NULL && *e == (char)0) return val; 
	return -1;
}

void aggiorna_flags(const char* str, size_t index)
{
	bool dummy = false;
	if ( str[0] == '-' )
	{
		for (size_t i = 1; i != strlen(str); i++)
		{
			switch (str[i])
			{
				case 'n':
					n_flag = true;
					n_index = index;
					dummy = false;
					break;
				case 'm':
					m_flag = true;
					m_index = index;
					dummy = false;
					break;
				case 'h':
					h_flag = true;
					dummy = false;
					break;
				case 's':
					s_flag = true;
					s_index = index;
					dummy = false;
					break;
				default:
					dummy = true;
					break;
			}
			if ( !dummy ) return;
		}
		unknown_index = index;
		unknown_flag = true;
	}
	return;
}

void parsing_cmd(int argc, char* argv[])
{
	char* dummy;
	int index;
	if ( h_flag )
	{
		printf(HELP);
		return;
	}
	if ( unknown_flag )
	{
		dummy = argv[unknown_index];
		printf("%s "SCONOSCIUTA, (dummy+1));
		return;
	}
	if ( n_flag )
	{
		if ((strlen(argv[n_index]) == 2 || isNumber(argv[n_index]) == -1) && argv[n_index+1][0] != '-') // e' della forma -n <numero>
			printf("-n: %ld\n", isNumber(argv[n_index + 1]));
		else // e' della forma -n<numero>
		{
			dummy = strchr(argv[n_index], 'n');
			index = (int)(dummy - argv[n_index]);
			dummy = strncpy(dummy, &(argv[n_index][index+1]), strlen(argv[n_index] - index + 1));
			if (strcmp(dummy, "")) 
				printf("-n: %s\n", dummy);
			else
			{
				printf("-n: "ERRORE);
				return;
			}
		}
	}
	if ( s_flag )
	{
		if (strlen(argv[s_index]) == 2)
			printf("-s: %s\n", argv[s_index+1]);
		else // e' della forma -s<string>
		{
			dummy = strchr(argv[s_index], 's');
			index = (int)(dummy - argv[s_index]);
			printf("-s: %s\n", strncpy(dummy, &(argv[s_index][index+1]), strlen(argv[s_index] - index + 1)));}
	}
	if ( m_flag )
	{
		if ((strlen(argv[m_index]) == 2 || isNumber(argv[m_index]) == -1) && argv[m_index+1][0] != '-') // e' della forma -m <numero>
			printf("-m: %ld\n", isNumber(argv[m_index + 1]));
		else // e' della forma -m<numero>
		{
			dummy = strchr(argv[m_index], 'm');
			index = (int)(dummy - argv[m_index]);
			dummy = strncpy(dummy, &(argv[m_index][index+1]), strlen(argv[m_index] - index + 1));
			if (strcmp(dummy, ""))
				printf("-m: %s\n", dummy);
			else
			{
				printf("-m: "ERRORE);
				return;
			}
		}
	}
	return;
}