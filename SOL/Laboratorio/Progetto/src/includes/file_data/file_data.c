#include <file_data.h>
#include <string.h>

struct filedata
{
	int descriptor;
	char* name;
	size_t size;
	void* contents;
	int* lockedby;
};

int* file_islocked(const struct filedata* file)
{
	return file->lockedby;
}

size_t file_getsize(const struct filedata* file)
{
	return file->size;
}

struct filedata* filedata_init(int filedescriptor, const char* filename, const int* lockedby,
			const void* contents, size_t size)
{
	struct filedata* res = (struct filedata*) malloc(sizeof(struct filedata));
	if (res == NULL) return NULL;
	res->name = (char*) calloc(strlen(filename) + 1, sizeof(char));
	if (res->name == NULL) return NULL;
	memcpy(res->name, filename, strlen(filename));
	if (size != 0 && contents != NULL)
	{
		res->contents = malloc(size);
		if (res->contents == NULL) return NULL;
		memcpy(res->contents, contents, size);
	}
	res->descriptor = filedescriptor;
	res->size = size;
	res->lockedby = lockedby;
	return res;
}

int file_lock(struct filedata* file, const int* lockedby)
{
	if (file->lockedby != NULL) return -1;
	file->lockedby = lockedby;
	return 0;
}

int file_unlock(struct filedata* file)
{
	if (file->lockedby == NULL) return -1;
	file->lockedby = NULL;
	return 0;
}

int file_setcontents(struct filedata* file, const void* buffer, size_t size)
{
	if (file->contents != NULL && file->size != 0) return -1;
	file->contents = (void*) malloc(size);
	if (file->contents == NULL) return -1;
	memcpy(file->contents, buffer, size);
	file->size = size;
	return 0;
}

int file_addcontents(struct filedata* file, const void* buffer, size_t size)
{
	if (file->size == 0) return file_setcontents(file, buffer, size);
	realloc(file->contents, file->size + size);
	if (file->contents == NULL) return -1;
	memcpy(file->contents + file->size, buffer, size);
	file->size += size;
	return 0;
}

void file_delete(struct filedata* file)
{
	free(file->contents);
	free(file->name);
	free(file);
}

void* fileGetContents(const struct filedata* file, size_t* size)
{
	void* contents = malloc(file->size);
	if (contents == NULL) return NULL;
	memcpy(contents, file->contents, file->size);
	*size = file->size;
	return contents;
}