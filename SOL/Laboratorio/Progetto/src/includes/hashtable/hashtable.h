#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

#include <stdlib.h>

#define HASHTABLE_INITIALIZER(buckets_no) hashtableInit(buckets_no, NULL, NULL);

// Struct fields are not exposed in order to maintain the struct's invariant properties.
typedef struct hashtable_entry hashtable_entry_t;
// Struct fields are not exposed in order to maintain the struct's invariant properties.
typedef struct hashtable hashtable_t;

size_t hashFunction(const void*);
int hashCompare(const void*, const void*);

hashtable_t* hashtableInit(size_t, size_t (const void*), int (const void*, const void*));
int hashtableInsert(hashtable_t*, const void*, size_t, const void*, size_t);
int hashtableFind(const hashtable_t*, const void*);
void* hashtableGetEntry(const hashtable_t*, const void*);
int hashtableDeleteNode(hashtable_t*, const void*);
void hashtableFree(hashtable_t*);
void hashtablePrint(const hashtable_t*);

#endif