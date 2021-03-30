#ifndef _UTILITIES_H
#define _UTILITIES_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX_ERROR_MSG_SIZE 1024

/**
 * @brief wrapper for system calls.
 * @param name syscall's name.
 * @param value variable syscall's result will be saved in.
 * @param system_call actual system call with all its parameters.
 * @param error_message error message going to stderr.
*/
#define SYSCALL(name, value, system_call, error_message, ...) \
{ \
	do \
	{ \
		if ((value = system_call) == -1) \
		{ \
			perror(#name); \
			int aux_errno = errno; \
			print_errors(error_message, __VA_ARGS__); \
			exit(aux_errno); \
		} \
	} while(0); \
}

// handling error in fopen
#define FOPEN(fname, path, mode) \
{ \
	do \
	{ \
		if ((fname = fopen(path, mode)) == NULL) \
		{ \
			fprintf(stderr, "File %s could not be opened in given mode <%s>.\n", path, mode); \
			return 1; \
		} \
	} while(0); \
}

static inline void print_errors(char* msg, ...)
{
	char tmp[] = "ERROR: ";
	va_list args;
	char* err = (char*) malloc(strlen(msg) + strlen(tmp) + MAX_ERROR_MSG_SIZE);
	if (!err)
	{
		perror("malloc");
		fprintf(stderr, "malloc failed in print_errors.\n");
		return;
	}
	memcpy(err, tmp, strlen(tmp));
	memcpy(err + strlen(tmp), msg, strlen(msg));
	va_start (args, msg);
	vfprintf(stderr, err, args);
	va_end (args);
	free(err);
}


static inline int isNumber(const char* s, long* n)
{
	if (s == NULL) return 1;
	if (strlen(s) == 0) return 1;
	char* e = NULL;
	errno = 0;
	long val = strtol(s, &e, 10);
	if (errno == ERANGE) return 2; // overflow/underflow
	if (e != NULL && *e == (char)0)
	{
		*n = val;
		return 0; // successo 
	}
	return 1; // non e' un numero
}

#endif