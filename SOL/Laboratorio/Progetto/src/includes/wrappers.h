#ifndef _WRAPPERS_H_
#define _WRAPPERS_H_

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
/**
 * @brief Exits with exit value 1 if filename cannot be opened in given mode.
 * @param file_ptr (will be set to) opened file pointer.
 * @param filename name of the file to be opened.
 * @param mode mode the file is to be opened with.
*/
#define FOPEN(file_ptr, filename, mode) \
	do \
	{ \
		if ((file_ptr = fopen(filename, mode)) == NULL) \
		{ \
			fprintf(stderr, "File %s could not be opened in given mode <%s>.\n", filename, mode); \
			exit(EXIT_FAILURE); \
		} \
	} while(0);

/**
 * @brief Exits with exit value 1 if variable has not been set to expected_value
 * after function_call.
 * @param variable (will be set to) function_call output value
 * @param expected_value value (usually) returned by function_call in case of success
 * @param function_call actual function call
 * @param error_message error message describing what errno has been set by
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

/**
 * @brief Exits with exit value 1 if variable has been set to expected_value after function_call.
 * @param variable (will be set to) function_call output value
 * @param expected_value value (usually) returned by function_call in case of failure
 * @param function_call actual function call
 * @param error_message error message (usually describing what errno has been set by)
*/
#define EXIT_IF_EQ(variable, expected_value, function_call, error_message) \
	do \
	{ \
		if ((variable = function_call) == expected_value) \
		{ \
			perror(#error_message); \
			exit(EXIT_FAILURE); \
		} \
	} while(0);

/**
 * @brief Returns with value expected_value if variable has been set to 
 * expected_value after function_call.
 * @param variable (will be set to) function_call output value
 * @param expected_value value (usually) returned by function_call in case of success
 * @param function_call actual function call
 * @param error_message error message describing what errno has been set by
*/
#define RETURN_IF_EQ(variable, expected_value, function_call, error_message) \
	if ((variable = function_call) == expected_value) \
	{ \
		perror(#error_message); \
		return variable; \
	}

/**
 * @brief Exits with exit value 1 if variable has been set to NULL after function_call.
 * @param variable (will be set to) function_call output value
 * @param function_call actual function call
 * @param error_message error message (usually describing what errno has been set by)
*/
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