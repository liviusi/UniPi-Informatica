#ifndef _SERVER_API_H_
#define _SERVER_API_H_

#include <stdlib.h>
#include <errno.h>

enum flags
{
	O_CREATE = 0x01,
	O_LOCK = 0x10
};

int openConnection(const char* sockname, int msec, const struct timespec abstime);

int closeConnection(const char* sockname);

int openFile(const char* pathname, int flags);

int readFile(const char* pathname, void** buf, size_t* size);

int writeFile(const char* pathname, const char* dirname);

int appendToFile(const char* pathname, void* buf, size_t size, const char* dirname);

int lockFile(const char* pathname);

int unlockFile(const char* pathname);

int closeFile(const char* pathname);

int removeFile(const char* pathname);

#endif