#ifndef _UTILITITES_H_
#define _UTILITIES_H_

#include <pthread.h>
#include <stdio.h>
#include <errno.h>

// handling error in fopen
#define FOPEN(file_descriptor, pathname, mode) \
{ \
        do \
        { \
                if ((file_descriptor = fopen(pathname, mode)) == NULL) \
                { \
                        fprintf(stderr, "File %s could not be opened in given mode <%s>.\n", pathname, mode); \
                        return 1; \
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
			exit(EXIT_FAILURE); \
		} \
	} while(0);

#define EXIT_IF_NULL(variable, function_call, error_message) \
	do \
	{ \
 		if ((variable = function_call) == NULL) \
		{ \
			perror(#error_message); \
			exit(EXIT_FAILURE); \
		} \
	} while(0);

static void Pthread_create(pthread_t* thread, const pthread_attr_t* attr, void* (*start_routine) (void*), void* arg)
{
	int err;
	if (( err = pthread_create(thread, attr, start_routine, arg)) != 0)
	{
		errno = err;
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}
}

static void Pthread_mutex_lock(pthread_mutex_t *mtx)
{
	int err;
	if (( err = pthread_mutex_lock(mtx) ) != 0)
	{
		errno = err;
		perror("lock");
		pthread_exit((void*) (long) errno);
	}
}

static void Pthread_mutex_unlock(pthread_mutex_t *mtx)
{
	int err;
	if (( err = pthread_mutex_unlock(mtx) ) != 0)
	{
		errno = err;
		perror("unlock");
		pthread_exit((void*) (long) errno);
	}
}

#endif