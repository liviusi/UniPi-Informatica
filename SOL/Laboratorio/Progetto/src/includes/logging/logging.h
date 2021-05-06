#ifndef _LOGGING_H_
#define _LOGGING_H_

#include <stdio.h>
#include <stdlib.h>

#define LOGGING_FILENAME "log.txt"
#define FOPEN(file_descriptor, pathname, mode) \
{ \
	do \
	{ \
		if ((file_descriptor = fopen(pathname, mode)) == NULL) \
		{ \
			fprintf(stderr, "File %s could not be opened in given mode <%s>.\n", pathname, mode); \
			exit(EXIT_FAILURE); \
		} \
	} while(0); \
}

// NULL errore
FILE* init_logging();

char write_log(FILE*, const char*, size_t);

int close_logging(FILE*);

// fopen logfile
// fwrite logfile
// fclose

#endif