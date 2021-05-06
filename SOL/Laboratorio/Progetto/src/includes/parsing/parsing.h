#ifndef _PARSING_H_
#define _PARSING_H_

#include <stdio.h>

#define BUFFERSIZE 128
#define NUMBER_THREADWORKERS "NUMEROTHREADWORKERS"
#define NUMBER_STORABLEFILES "FILEMEMORIZZABILI"
#define STORAGESIZE "SPAZIODIMEMORIZZAZIONE"
#define SOCKETFILENAME "NOMESOCKETFILE"
#define LOGFILENAME "NOMELOGFILE"

/**
 * @brief Reads config.txt file and parses its data.
 * @param config_filename config.txt path
 * @param nworkers (will point to the) number of thread workers
 * @param nfiles (will point to the) maximum amount of storable files
 * @param storage_size (will point to the) server's storage size
 * @param socket_filename (will point to the) socket filename
 * @param log_filename (will point to the) log filename
 * @return 0 on success, -1 on failure.
*/
int parse_config(const char*, size_t*, size_t*, size_t*, char**, char**);

#endif