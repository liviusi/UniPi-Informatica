#ifndef _UTILITITES_H_
#define _UTILITIES_H_

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
			exit(EXIT_FAILURE); \
		} \
	} while(0);

#define EXIT_IF_EXPECTED_VALUE(variable, expected_value, function_call, error_message) \
	do \
	{ \
 		if ((variable = function_call) == expected_value) \
		{ \
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



#endif