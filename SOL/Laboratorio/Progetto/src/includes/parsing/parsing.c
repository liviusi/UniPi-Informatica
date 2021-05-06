#include "parsing.h"

#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define BUFFERSIZE 128
#define NEGATIVE_VALUE_ERROR_MESSAGE " is not a valid positive integer.\n"
#define INVALID_FORMAT_ERROR_MESSAGE " does not follow the given format.\n"

#define PARSE_AND_ASSIGN_SIZE_T(buffer, target, variable) \
	do \
	{ \
		separator_position = strchr(buffer, separator); \
		if (separator_position == NULL) \
		{ \
			fprintf(stderr, target FORMAT_ERROR_MESSAGE); \
			return -1; \
		} \
		*separator_position = '\0'; \
		if (string_to_size_t(separator_position + 1, variable) != 0) \
		{ \
			fprintf(stderr, target NOT_VALID_ERROR_MESSAGE); \
			return -1; \
		} \
	} while(0)

#define PARSE_AND_ASSIGN_STRING(buffer, target, variable) \
	do \
	{ \
		separator_position = strchr(buffer, separator); \
		if (separator_position == NULL) \
		{ \
			fprintf(stderr, target FORMAT_ERROR_MESSAGE); \
			return -1; \
		} \
		*separator_position = '\0'; \
		memcpy(variable, separator_position + 1, BUFFERSIZE); \
		variable[strcspn(variable, "\n")] = '\0'; \
	} while(0)

char string_to_size_t(const char* s, size_t* n)
{
	if (s == NULL) return 1;
	if (strlen(s) == 0) return 1;
	char* e = NULL;
	errno = 0;
	long long val = strtoll(s, &e, 10);
	if (errno == ERANGE) return 2; // overflow/underflow
	if (e != NULL && *e == (char)0)
	{
		if (val < 0) return 1; // non valido
		*n = val;
		return 0; // successo 
	}
	return 1; // non e' un numero
}

int parse_config(const char* config_filename, size_t* nworkers, size_t* nfiles, size_t* storage_size, char** socket_filename, char** log_filename)
{
	char* buffer = (char*) calloc(BUFFERSIZE, sizeof(char));
	if (buffer == NULL)
	{
		fprintf(stderr, "[parse_config] Could not allocate buffer for parsing.\n");
		return -1;
	}
	char* ptr = buffer;
	FILE* config_file;
	if ((config_file = fopen(config_filename, "r")) == NULL)
	{
		perror("[parse_config] fopen");
		return -1;
	}
	const char separator = '=';
	char* separator_position = NULL;
	const char str_numb_threadworkers[] = "NUMEROTHREADWORKERS";
	const char str_numb_storablefiles[] = "FILEMEMORIZZABILI";
	const char str_storagesize[] = "SPAZIODIMEMORIZZAZIONE";
	const char str_socketfilename[] = "NOMESOCKETFILE";
	const char str_logfilename[] = "NOMELOGFILE";
	while ((buffer = fgets(buffer, BUFFERSIZE, config_file)) != NULL)
	{
		if (strncmp(buffer, str_numb_threadworkers, sizeof(str_numb_threadworkers)) == 0)
		{
			PARSE_AND_ASSIGN_SIZE_T(buffer, str_numb_threadworkers, nworkers);
			continue;
		}
		if (strncmp(buffer, str_numb_storablefiles, sizeof(str_numb_storablefiles)) == 0)
		{
			PARSE_AND_ASSIGN_SIZE_T(buffer, str_numb_storablefiles, nfiles);
			continue;
		}
		if (strncmp(buffer, str_storagesize, sizeof(str_storagesize)) == 0)
		{
			PARSE_AND_ASSIGN_SIZE_T(buffer, str_storagesize, storage_size);
			continue;
		}
		if (strncmp(buffer, str_socketfilename, sizeof(str_socketfilename)) == 0)
		{
			*socket_filename = (char*) calloc(BUFFERSIZE, sizeof(char));
			if (*socket_filename == NULL)
			{
				fprintf(stderr, "[parse_config] Could not allocate buffer for socket's filename.\n");
				return -1;
			}
			PARSE_AND_ASSIGN_STRING(buffer, str_socketfilename, (*socket_filename));
			continue;
		}
		if (strncmp(buffer, str_logfilename, sizeof(str_logfilename)) == 0)
		{
			*log_filename = (char*) calloc(BUFFERSIZE, sizeof(char));
			if (*log_filename == NULL)
			{
				fprintf(stderr, "[parse_config] Could not allocate buffer for log's filename.\n");
				return -1;
			}
			PARSE_AND_ASSIGN_STRING(buffer, str_logfilename, (*log_filename));
			continue;
		}
	}
	free(ptr);
	return 0;
}