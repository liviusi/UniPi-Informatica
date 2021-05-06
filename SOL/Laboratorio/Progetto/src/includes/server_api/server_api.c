#include "server_api.h"
#include <time.h>

int openConnection(const char* sockname, int msec, const struct timespec abstime)
{
	return 0;
}

int closeConnection(const char* sockname)
{
	return 0;
}

int openFile(const char* pathname, int flags)
{
	return 0;
}

int readFile(const char* pathname, void** buf, size_t* size)
{
	return 0;
}

int writeFile(const char* pathname, const char* dirname)
{
	return 0;
}

int appendToFile(const char* pathname, void* buf, size_t size, const char* dirname)
{
	return 0;
}

int lockFile(const char* pathname)
{
	return 0;
}

int unlockFile(const char* pathname)
{
	return 0;
}

int closeFile(const char* pathname)
{
	return 0;
}

int removeFile(const char* pathname)
{
	return 0;
}