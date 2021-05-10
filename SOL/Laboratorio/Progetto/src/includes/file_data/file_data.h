#ifndef _FILE_DATA_H_
#define _FILE_DATA_H_

#include <stdbool.h>
#include <stdlib.h>

#define FILEDATA_INIT(fd, name, lock) filedata_init(fd, name, lock, NULL, 0);

struct filedata;

bool file_islocked(const struct filedata*);
size_t file_getsize(const struct filedata*);
struct filedata* filedata_init(int, const char*, bool, const void*, size_t);
int file_lock(struct filedata*);
int file_unlock(struct filedata*);
int file_setcontents(struct filedata*, const void*, size_t);
int file_addcontents(struct filedata*, const void*, size_t);
void file_delete(struct filedata*);


#endif