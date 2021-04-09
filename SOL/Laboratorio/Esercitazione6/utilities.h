#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include <errno.h>
#include <stdlib.h>
#include <string.h>

static inline char isNumber(const char* s, unsigned int* n)
{
	if (s == NULL) return 1;
	if (strlen(s) == 0) return 1;
	char* e = NULL;
	errno = 0;
	long val = strtol(s, &e, 10);
	if (errno == ERANGE) return 2; // overflow/underflow
	if (e != NULL && *e == (char)0)
	{
			*n = val;
			return 0; // successo 
	}
	return 1; // non e' un numero
}

#endif