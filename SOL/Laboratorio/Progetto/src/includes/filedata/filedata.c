#include <filedata.h>
#include <bst.h>
#include <string.h>

struct filedata_t
{
	int descriptor;
	char* name;
	size_t size;
	void* contents;
	int* lockedby;
	struct tree_node_t* openedby;
};

int* filedataGetLockedBy(const struct filedata_t* file)
{
	return file->lockedby;
}

size_t filedataGetSize(const struct filedata_t* file)
{
	return file->size;
}

struct filedata_t* filedataInit(int filedescriptor, const char* filename, const int* lockedby,
			const void* contents, size_t size)
{
	struct filedata_t* res = (struct filedata_t*) malloc(sizeof(struct filedata_t));
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
	res->openedby = NULL;
	return res;
}

int filedataSetLockedBy(struct filedata_t* file, const int* lockedby)
{
	if (file->lockedby != NULL) return -1;
	file->lockedby = lockedby;
	return 0;
}

int filedataUnlock(struct filedata_t* file)
{
	if (file->lockedby == NULL) return -1;
	file->lockedby = NULL;
	return 0;
}

bool filedataIsOpenedBy(const struct filedata_t* file, int descriptor)
{
	return bstFind(file->openedby, descriptor);
}

int filedataAddOpenedBy(struct filedata_t* file, int descriptor)
{
	if (file == NULL) return -1;
	if (bstInsert(&(file->openedby), descriptor) == -1) return -1;
	return 0;
}

int filedataRemoveOpenedBy(struct filedata_t* file, int descriptor)
{
	if (file == NULL) return -1;
	if (bstFind(file->openedby, descriptor) != 1) return -1;
	file->openedby = bstDeleteNode(file->openedby, descriptor);
	return 0;
}

int filedataSetContents(struct filedata_t* file, const void* buffer, size_t size)
{
	if (file->contents != NULL && file->size != 0) return -1;
	file->contents = (void*) malloc(size);
	if (file->contents == NULL) return -1;
	memcpy(file->contents, buffer, size);
	file->size = size;
	return 0;
}

int filedataAppendContents(struct filedata_t* file, const void* buffer, size_t size)
{
	if (file->size == 0) return filedataSetContents(file, buffer, size);
	realloc(file->contents, file->size + size);
	if (file->contents == NULL) return -1;
	memcpy(file->contents + file->size, buffer, size);
	file->size += size;
	return 0;
}

void filedataFree(struct filedata_t* file)
{
	free(file->contents);
	free(file->name);
	free(file);
}

void* fileGetContents(const struct filedata_t* file, size_t* size)
{
	void* contents = malloc(file->size);
	if (contents == NULL) return NULL;
	memcpy(contents, file->contents, file->size);
	*size = file->size;
	return contents;
}