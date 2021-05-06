#ifndef _LOGGING_H_
#define _LOGGING_H_

#include <stdio.h>
#include <stdlib.h>

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