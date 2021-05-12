#include <filedata.h>
#include <bst.h>
#include <string.h>

struct filedata
{
	int descriptor;
	char* name;
	size_t size;
	void* contents;
	int* lockedby;
	tree_node_t* openedby;
};

/**
 * @param file pointer to filedata_t
 * @returns pointer to the client descriptor of this file's lock owner.
 * 
*/
int* filedataGetLockedBy(const filedata_t* file)
{
	return file->lockedby;
}

/**
 * @param file pointer to filedata_t
 * @returns file content's size in bytes.
*/
size_t filedataGetSize(const filedata_t* file)
{
	return file->size;
}

/**
 * @brief Initializes filedata_t
 * @param filedescriptor file descriptor these metadata refers to
 * @param filename file name
 * @param lockedby pointer to the file descriptor of the client which is requesting
 * for this file's lock
 * @param contents blob of data containing the actual contents of the file
 * @param size contents' size
 * @returns pointer to initialized filedata_t struct. It returns NULL if and only if
 * required memory allocation fails (sets errno to ENOMEM).
*/
filedata_t* filedataInit(int filedescriptor, const char* filename, const int* lockedby,
			const void* contents, size_t size)
{
	filedata_t* res = (filedata_t*) malloc(sizeof(filedata_t));
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

/**
 * @brief Sets a lock to filedata_t owned by lockedby.
 * @param file pointer to filedata_t
 * @param lockedby pointer to the client's file descriptor of the lock owner
 * @returns 0 on success, -1 otherwise. It may fail because there already is a
 * lock owner (sets errno to ).
*/
int filedataSetLockedBy(filedata_t* file, const int* lockedby)
{
	if (file->lockedby != NULL) return -1;
	file->lockedby = lockedby;
	return 0;
}

/**
 * @brief Removes the lock to filedata_t
 * @param file pointer to filedata_t
 * @returns 0 on success, -1 otherwise. It may fail because there is no lock owner.
*/
int filedataUnlock(filedata_t* file)
{
	if (file->lockedby == NULL) return -1;
	file->lockedby = NULL;
	return 0;
}

/**
 * @brief Returns true if and only if descriptor has already opened file.
 * @param file pointer to filedata_t
 * @param descriptor client's file descriptor of the lock's owner
 * @returns true if and only if file has been opened by descriptor (and has yet to be closed).
*/
bool filedataIsOpenedBy(const filedata_t* file, int descriptor)
{
	return bstFind(file->openedby, descriptor);
}

/**
 * @brief Adds descriptor to the set of the clients which have opened file.
 * @param file pointer to filedata_t
 * @param descriptor file descriptor of the client attempting to open file
 * @returns 0 on success, -1 otherwise. It may fail either because file is NULL or
 * descriptor already belongs to the set of the clients which have opened file.
*/
int filedataAddOpenedBy(filedata_t* file, int descriptor)
{
	if (file == NULL) return -1;
	if (bstInsert(&(file->openedby), descriptor) == -1) return -1;
	return 0;
}

/**
 * @brief Removes descriptor from the set of the clients which have opened file.
 * @param file pointer to filedata_t
 * @param descriptor file descriptor of the client attempting to close file
 * @returns 0 on success, -1 otherwise. It may fail either because file is NULL or
 * descriptor does not belong to the set of the clients which have opened file.
*/
int filedataRemoveOpenedBy(filedata_t* file, int descriptor)
{
	if (file == NULL) return -1;
	if (bstFind(file->openedby, descriptor) != 1) return -1;
	file->openedby = bstDeleteNode(file->openedby, descriptor);
	return 0;
}

/**
 * @brief Sets file's contents to buffer.
 * @param file pointer to filedata_t
 * @param buffer blob of data the file's contents will be set to
 * @param size size of buffer
 * @returns 0 on success, -1 otherwise. It may fail because of memory allocation
 * (sets errno to ENOMEM).
*/
int filedataSetContents(filedata_t* file, const void* buffer, size_t size)
{
	if (file->contents != NULL && file->size != 0) return -1;
	file->contents = (void*) malloc(size);
	if (file->contents == NULL) return -1;
	memcpy(file->contents, buffer, size);
	file->size = size;
	return 0;
}

/**
 * @brief Appends buffer to file's contents.
 * @param file pointer to filedata_t
 * @param buffer blob of data to be appended to file's contents
 * @param size size of buffer
 * @returns 0 on success, -1 otherwise. It may fail because of memory allocation
 * (sets errno to ENOMEM).
*/
int filedataAppendContents(filedata_t* file, const void* buffer, size_t size)
{
	if (file->size == 0) return filedataSetContents(file, buffer, size);
	realloc(file->contents, file->size + size);
	if (file->contents == NULL) return -1;
	memcpy(file->contents + file->size, buffer, size);
	file->size += size;
	return 0;
}

/**
 * @brief Frees memory allocated by file.
 * @param file pointer to filedata_t
*/
void filedataFree(filedata_t* file)
{
	free(file->contents);
	free(file->name);
	free(file);
}

/**
 * @param file pointer to filedata_t
 * @param size pointer which will refer to file's contents buffer size.
 * @returns Get a heap-allocated copy of the file's contents. Sets size to file's
 * contents buffer size if and only if size is not NULL. It may return NULL because of memory
 * allocation failure (sets errno to ENOMEM).
*/
void* fileGetContents(const filedata_t* file, size_t* size)
{
	void* contents = malloc(file->size);
	if (contents == NULL) return NULL;
	memcpy(contents, file->contents, file->size);
	if (size != NULL) *size = file->size;
	return contents;
}