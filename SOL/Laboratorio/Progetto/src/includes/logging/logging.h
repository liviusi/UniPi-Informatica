#ifndef _LOGGING_H_
#define _LOGGING_H_

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Initializes pointer to file used for logging.
 * @param log_filename log's filename.
 * @return pointer to FILE* used for logging, NULL on failure.
*/
FILE* init_logging(const char*);

/**
 * @brief Writes buffer in log.
 * @param log_file pointer to initialized log file.
 * @param buffer buffer of characters to write inside log.
 * @return 0 on success, -1 on failure.
*/
char write_log(FILE*, const char*);

/**
 * @brief Deallocate resources used for logging (i.e. closes file used for logging).
 * @param log_file pointer to initialized log file.
 * @return 0 on success, any other integer otherwise.
*/
int close_logging(FILE*);

#endif