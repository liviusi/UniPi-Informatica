#ifndef _WRAPPERS_H_
#define _WRAPPERS_H_

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
/**
 * @brief Exits with exit value 1 if filename cannot be opened.
 * @param file_ptr pointer to FILE*.
 * @param filename file name.
 * @param mode mode to open the file with.
*/
#define FOPEN(file_ptr, filename, mode) \
{ \
	do \
	{ \
		if ((file_ptr = fopen(filename, mode)) == NULL) \
		{ \
			fprintf(stderr, "File %s could not be opened in given mode <%s>.\n", filename, mode); \
			exit(EXIT_FAILURE); \
		} \
	} while(0); \
}

/**
 * @brief Exits with exit value 1 if variable is not equal to expected_value.
*/
#define EXIT_IF_NEQ(variable, expected_value, function_call, error_message) \
	do \
	{ \
		if ((variable = function_call) != expected_value) \
		{ \
			errno = variable; \
			perror(#error_message); \
			exit(EXIT_FAILURE); \
		} \
	} while(0);

#define EXIT_IF_EQ(variable, expected_value, function_call, error_message) \
	do \
	{ \
		if ((variable = function_call) == expected_value) \
		{ \
			perror(#error_message); \
			exit(EXIT_FAILURE); \
		} \
	} while(0);

#define RETURN_IF_EQ(variable, expected_value, function_call, error_message) \
	if ((variable = function_call) == expected_value) \
	{ \
		perror(#error_message); \
		return variable; \
	}

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