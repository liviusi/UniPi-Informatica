#ifndef _filedata_H_
#define _filedata_H_

#include <stdbool.h>
#include <stdlib.h>

/**
 * @brief Initializes filedata_t metadata of a file with no contents.
 * @param fd file descriptor
 * @param name file name
 * @param lock pointer to lock owner
 * @returns pointer to filedata_t.
*/
#define FILEDATA_INIT(fd, name, lock) filedataInit(fd, name, lock, NULL, 0);

// Struct fields are not exposed in order to maintain the struct's invariant properties.
typedef struct filedata filedata_t;

int* filedataGetLockedBy(const filedata_t*);
size_t filedataGetSize(const filedata_t*);
filedata_t* filedataInit(int, const char*, const int*, const void*, size_t);
int filedataSetLockedBy(filedata_t*, const int*);
int filedataUnlock(filedata_t*);
bool filedataIsOpenedBy(const filedata_t*, int);
int filedataAddOpenedBy(filedata_t*, int);
int filedataRemoveOpenedBy(filedata_t*, int);
int filedataSetContents(filedata_t*, const void*, size_t);
int filedataAppendContents(filedata_t*, const void*, size_t);
void* fileGetContents(const filedata_t*, size_t*);
void filedataFree(filedata_t*);


#endif