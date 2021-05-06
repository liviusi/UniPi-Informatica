#ifndef _PARSING_H_
#define _PARSING_H_

#include <stdio.h>

#define BUFFERSIZE 128
#define NUMBER_THREADWORKERS "NUMEROTHREADWORKERS"
#define NUMBER_STORABLEFILES "FILEMEMORIZZABILI"
#define STORAGESIZE "SPAZIODIMEMORIZZAZIONE"
#define SOCKETFILENAME "NOMESOCKETFILE"
#define LOGFILENAME "NOMELOGFILE"

// 0 successo
// -1 altrimenti
int parse_config(const char*, size_t*, size_t*, size_t*, char**, char**);

#endif