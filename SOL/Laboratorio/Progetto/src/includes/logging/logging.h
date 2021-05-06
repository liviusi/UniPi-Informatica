#ifndef _LOGGING_H_
#define _LOGGING_H_

#include <stdio.h>
#include <stdlib.h>

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
 * @param log_filename log's filename.
 * @return pointer to FILE* used for logging on success, NULL on failure.
*/
FILE* init_logging(const char*);

// 0 successo
// -1 altrimenti
char write_log(FILE*, const char*);

// 0 successo
// -1 altrimenti
int close_logging(FILE*);

#endif