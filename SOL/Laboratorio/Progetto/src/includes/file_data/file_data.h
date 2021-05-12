#ifndef _FILE_DATA_H_
#define _FILE_DATA_H_

#include <stdbool.h>
#include <stdlib.h>

#define FILEDATA_INIT(fd, name, lock) filedataInit(fd, name, lock, NULL, 0);

struct filedata_t;

int* filedataGetLockedBy(const struct filedata_t*);
size_t filedataGetSize(const struct filedata_t*);
struct filedata_t* filedataInit(int, const char*, const int*, const void*, size_t);
int filedataSetLockedBy(struct filedata_t*, const int*);
int filedataUnlock(struct filedata_t*);
bool filedataIsOpenedBy(const struct filedata_t*, int);
int filedataAddOpenedBy(struct filedata_t*, int);
int filedataRemoveOpenedBy(struct filedata_t*, int);
int filedataSetContents(struct filedata_t*, const void*, size_t);
int filedataAppendContents(struct filedata_t*, const void*, size_t);
void* fileGetContents(const struct filedata_t*, size_t*);
void filedataFree(struct filedata_t*);


#endif