#ifndef _UTILITITES_H_
#define _UTILITIES_H_

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

// handling error in fopen
#define FOPEN(file_descriptor, pathname, mode) \
{ \
	do \
	{ \
			if ((file_descriptor = fopen(pathname, mode)) == NULL) \
			{ \
					fprintf(stderr, "File %s could not be opened in given mode <%s>.\n", pathname, mode); \
					exit(1); \
			} \
	} while(0); \
}

#define EXIT_IF_NOT_EXPECTED_VALUE(variable, expected_value, function_call, error_message) \
	do \
	{ \
 		if ((variable = function_call) != expected_value) \
		{ \
			errno = variable; \
			perror(#error_message); \
			exit(1); \
		} \
	} while(0);

#define EXIT_IF_NULL(variable, function_call, error_message) \
	do \
	{ \
 		if ((variable = function_call) == NULL) \
		{ \
			perror(#error_message); \
			exit(1); \
		} \
	} while(0);

void Pthread_create(pthread_t* thread, const pthread_attr_t* attr, void* (*start_routine) (void*), void* arg);
void Pthread_mutex_lock(pthread_mutex_t *mtx);
void Pthread_mutex_unlock(pthread_mutex_t *mtx);

#endif