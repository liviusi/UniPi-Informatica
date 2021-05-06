#include "parsing.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define NOT_VALID_ERROR_MESSAGE " is not a valid value.\n"

#define PARSE_AND_ASSIGN_SIZE_T(buffer, variable, error_message) \
	do \
	{ \
		buffer[strcspn(buffer, "\n")] = '\0'; \
		separator_position = strchr(buffer, separator); \
		if (separator_position == NULL) \
		{ \
			fprintf(stderr, error_message); \
			return -1; \
		} \
		*separator_position = '\0'; \
		if (string_to_size_t(separator_position + 1, variable) != 0) \
		{ \
			fprintf(stderr, error_message); \
			return -1; \
		} \
	} while(0)

#define PARSE_AND_ASSIGN_STRING(buffer, variable, error_message) \
	do \
	{ \
		separator_position = strchr(buffer, separator); \
		if (separator_position == NULL) \
		{ \
			fprintf(stderr, error_message); \
			return -1; \
		} \
		*separator_position = '\0'; \
		memcpy(variable, separator_position + 1, strlen(separator_position+1)); \
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
		if (val < 0) return -1; // non valido
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
	while ((buffer = fgets(buffer, BUFFERSIZE, config_file)) != NULL)
	{
		if (strncmp(buffer, NUMBER_THREADWORKERS, strlen(NUMBER_THREADWORKERS)) == 0)
		{
			PARSE_AND_ASSIGN_SIZE_T(buffer, nworkers, NUMBER_THREADWORKERS NOT_VALID_ERROR_MESSAGE);
			continue;
		}
		if (strncmp(buffer, NUMBER_STORABLEFILES, strlen(NUMBER_STORABLEFILES)) == 0)
		{
			PARSE_AND_ASSIGN_SIZE_T(buffer, nfiles, NUMBER_STORABLEFILES NOT_VALID_ERROR_MESSAGE);
			continue;
		}
		if (strncmp(buffer, STORAGESIZE, strlen(STORAGESIZE)) == 0)
		{
			PARSE_AND_ASSIGN_SIZE_T(buffer, storage_size, STORAGESIZE NOT_VALID_ERROR_MESSAGE);
			continue;
		}
		if (strncmp(buffer, SOCKETFILENAME, strlen(SOCKETFILENAME)) == 0)
		{
			*socket_filename = (char*) calloc(BUFFERSIZE, sizeof(char));
			if (*socket_filename == NULL)
			{
				fprintf(stderr, "[parse_config] Could not allocate buffer for socket's filename.\n");
				return -1;
			}
			PARSE_AND_ASSIGN_STRING(buffer, (*socket_filename), SOCKETFILENAME NOT_VALID_ERROR_MESSAGE);
			continue;
		}
		if (strncmp(buffer, LOGFILENAME, strlen(LOGFILENAME)) == 0)
		{
			*log_filename = (char*) calloc(BUFFERSIZE, sizeof(char));
			if (*log_filename == NULL)
			{
				fprintf(stderr, "[parse_config] Could not allocate buffer for log's filename.\n");
				return -1;
			}
			PARSE_AND_ASSIGN_STRING(buffer, (*log_filename), LOGFILENAME NOT_VALID_ERROR_MESSAGE);
			continue;
		}
	}
	free(ptr);
	if (fclose(config_file) != 0)
	{
		perror("[parse_config] fclose");
		return 1;
	}
	return 0;
}

/* int main(void)
{
	size_t nworkers = 0, nfiles = 0, storage_size = 0;
	char* socket_filename = NULL;
	char* log_filename = NULL;
	parse_config("config.txt", &nworkers, &nfiles, &storage_size, &socket_filename, &log_filename);

	fprintf(stdout, "nworkers = %lu\nnfiles = %lu\nstorage_size = %lu\n", nworkers, nfiles, storage_size);
	fprintf(stdout, "socket_filename = %s\nlog_filename = %s\n", socket_filename, log_filename);

	free(log_filename);
	free(socket_filename);
} */