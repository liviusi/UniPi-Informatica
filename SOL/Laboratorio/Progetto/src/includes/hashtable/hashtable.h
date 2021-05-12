#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

#include <stdlib.h>

#define HASHTABLE_INITIALIZER(buckets_no) hashtableInit(buckets_no, NULL, NULL);

struct hashtable_entry_t;
struct hashtable_t;

size_t hashFunction(const void*);
int hashCompare(const void*, const void*);

struct hashtable_t* hashtableInit(size_t, size_t (const void*), int (const void*, const void*));
int hashtableInsert(struct hashtable_t*, const void*, size_t, const void*, size_t);
int hashtableFind(const struct hashtable_t*, const void*);
void* hashtableGetEntry(const struct hashtable_t*, const void*);
int hashtableDeleteNode(struct hashtable_t*, const void*);
void hashtableFree(struct hashtable_t*);
void hashtablePrint(const struct hashtable_t*);

#endif