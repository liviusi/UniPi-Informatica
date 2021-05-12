#include <hashtable.h>
#include <wrappers.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

struct hashtable_entry
{
	void* key;
	void* data;
	size_t data_size;
	struct hashtable_entry* next;
};

struct hashtable
{
	size_t buckets_no;
	hashtable_entry_t** buckets; 
	size_t (*hash_function) (const void*);
	int (*hash_compare) (const void*, const void*);
};

/**
 * @brief Polynomial rolling hash function.
 * @param buffer buffer to hash
 * @return hash value of input buffer
*/
size_t hashFunction(const void* buffer)
{
	if (buffer == NULL) return 0;
	char* tmp = (char*) buffer;
	// roughly equal to the number of letters
	// in the alphabet (uppercase and lowercase)
	const int p = 53;
	const int m = 1e9 + 9; // large prime number
	size_t hash_value = 0;
	long long p_pow = 1;
	for (size_t i = 0; tmp[i] != '\0'; i++)
	{
		char c = tmp[i];
		hash_value = (hash_value + (c - 'a' + 1) * p_pow) % m;
		p_pow = (p_pow * p) % m;
	}
	return hash_value;
}

/**
 * @brief String comparison.
*/
int hashCompare(const void* a, const void* b)
{
	char* buffer1 = (char*) a;
	char* buffer2 = (char*) b;
	return strcmp(buffer1, buffer2);
}

/**
 * @brief Creates hashtable entry.
 * @param key entry key
 * @param key_size size of key
 * @param data entry blob of data
 * @param data_size size of data
 * @returns pointer to entry. Key and data will be a heap-allocated copy of the given parameters.
 * NULL may be returned if and only if memory allocation fails (sets errno to ENOMEM).
*/
hashtable_entry_t* entryCreate(const void* key, size_t key_size, 
		const void* data, size_t data_size)
{
	hashtable_entry_t* entry;
	RETURN_IF_EQ(entry, NULL, (hashtable_entry_t*) 
			malloc(sizeof(hashtable_entry_t)), malloc);
	RETURN_IF_EQ(entry->key, NULL, malloc(key_size), malloc);
	if (data != NULL && data_size > 0)
	{
		RETURN_IF_EQ(entry->data, NULL, malloc(data_size), malloc);
		memcpy(entry->data, data, data_size);
		entry->data_size = data_size;
	}
	else
	{
		entry->data = NULL;
		entry->data_size = 0;
	}
	memcpy(entry->key, key, key_size);
	entry->next = NULL;
	return entry;
}

/**
 * @brief Print out entry data. Used when debugging.
*/
void entryPrint(const hashtable_entry_t* entry)
{
	if (entry == NULL) fprintf(stdout, "NULL\n");
	else
	{
		fprintf(stdout, "(%s ; %s) -> ", (char*) entry->key, (char*) entry->data);
		entryPrint(entry->next);
	}
}

/**
 * @brief Initializes hashtable.
 * @param buckets_no number of buckets
 * @param hash_function pointer to the function used for hashing; if NULL is passed then
 * it uses a default provided polynomial rolling hash function.
 * @param hash_compare pointer to the function used for key comparison; if NULL is passed then
 * it uses a default provided string comparison function.
 * @returns pointer to initialized hashtable. NULL may be returned if and only if
 * memory allocation fails (sets errno to ENOMEM).
*/
hashtable_t* hashtableInit(size_t buckets_no, size_t (*hash_function) (const void*), 
		int (*hash_compare) (const void*, const void*))
{
	hashtable_t* table;
	RETURN_IF_EQ(table, NULL, (hashtable_t*) malloc(sizeof(hashtable_t)), malloc);
	table->buckets_no = buckets_no;
	table->buckets = (hashtable_entry_t**) 
			malloc(sizeof(hashtable_entry_t*) * buckets_no);
	if (table->buckets == NULL)
	{
		perror("malloc");
		return NULL;
	}
	for (size_t i = 0; i < buckets_no; i++) table->buckets[i] = NULL;
	table->hash_function = ((hash_function == NULL) ? (hashFunction) : (hash_function));
	table->hash_compare = ((hash_compare == NULL) ? (hashCompare) : (hash_compare));
	return table;
}

/**
 * @brief Creates and inserts entry into hashtable. Duplicates are not allowed.
 * @param table pointer to hashtable
 * @param key pointer to key
 * @param key_size size of key
 * @param data pointer to blob of data
 * @param data_size size of data
 * @returns 0 on success, -1 otherwise. It may fail either because of memory allocation failure
 * (sets errno to ENOMEM) or because key and/or key_size are not valid.
*/
int hashtableInsert(hashtable_t* table, const void* key, size_t key_size,
		const void* data, size_t data_size)
{
	if (key == NULL || key_size == 0) return -1;
	size_t hash = table->hash_function(key) % table->buckets_no;
	for (hashtable_entry_t* curr = table->buckets[hash]; curr != NULL; curr = curr->next)
	{
		if (table->hash_compare(curr->key, key) == 0) // duplicates are not allowed
			return 0;
	}
	hashtable_entry_t* entry;
	entry = entryCreate(key, key_size, data, data_size);
	if (entry == NULL)
	{
		errno = ENOMEM;
		return -1;
	}
	entry->next = table->buckets[hash];
	table->buckets[hash] = entry;
	return 0;
}

void* hashtableGetEntry(const hashtable_t* table, const void* key)
{
	if (table == NULL) return NULL;
	size_t hash = table->hash_function(key) % table->buckets_no;
	hashtable_entry_t* entry = table->buckets[hash];
	while (entry != NULL)
	{
		if (table->hash_compare(entry->key, key) == 0)
		{
			void* data = malloc(sizeof(entry->data_size));
			if (data == NULL)
			{
				errno = ENOMEM;
				return NULL;
			}
			memcpy(data, entry->data, entry->data_size);
			return data;
		}
		entry = entry->next;
	}
	return NULL;
}

int hashtableFind(const hashtable_t* table, const void* key)
{
	if (table == NULL) return 0;
	size_t hash = table->hash_function(key) % table->buckets_no;
	hashtable_entry_t* entry = table->buckets[hash];
	while (entry != NULL)
	{
		if (table->hash_compare(entry->key, key) == 0) return 1;
		entry = entry->next;
	}
	return 0;
}

int hashtableDeleteNode(hashtable_t* table, const void* key)
{
	if (hashtableFind(table, key) == 0) return 0; // nothing to delete
	size_t hash = table->hash_function(key) % table->buckets_no;
	hashtable_entry_t* curr = table->buckets[hash];
	hashtable_entry_t* prev = NULL;
	while(curr != NULL)
	{
		// this node is the one to be deleted
		if (table->hash_compare(curr->key, key) == 0)
		{
			if (prev == NULL) // curr is the first node
			{
				table->buckets[hash] = curr->next;
				free(curr->key);
				if (curr->data != NULL) free(curr->data);
				free(curr);
				return 1;
			}
			if (curr->next == NULL) // curr is the last node
			{
				prev->next = NULL;
				free(curr->key);
				if (curr->data != NULL) free(curr->data);
				free(curr);
				return 1;
			}
			prev->next = curr->next;
			free(curr->key);
			if (curr->data != NULL) free(curr->data);
			free(curr);
			return 1;
		}
		prev = curr;
		curr = curr->next;
	}
	return 0;
}

void hashtableFree(hashtable_t* table)
{
	if (table == NULL) return;
	for (size_t i = 0; i < table->buckets_no; i++)
	{
		hashtable_entry_t* curr = table->buckets[i];
		hashtable_entry_t* tmp;
		while (curr != NULL)
		{
			tmp = curr;
			curr = curr->next;
			free(tmp->key);
			if (tmp->data != NULL) free(tmp->data);
			free(tmp);
		}
	}
	free(table->buckets);
	free(table);
}

void hashtablePrint(const hashtable_t* table)
{
	if (table == NULL) return;
	fprintf(stdout, "buckets = %lu", table->buckets_no);
	for (size_t i = 0; i < table->buckets_no; i++)
	{
		fprintf(stdout, "%lu: ", i);
		entryPrint(table->buckets[i]);
	}
	return;
}

/* int main(void)
{
	hashtable_t* table;
	table = hashtableInit(10, NULL, NULL);
	const char key1[] = "a";
	const char key2[] = "ab";
	const char key3[] = "abc";
	const char key4[] = "abcd";
	const char key5[] = "abcde";
	const char key6[] = "abcdef";
	const char data1[] = "f";
	const char data2[] = "fe";
	const char data3[] = "fed";
	const char data4[] = "fedc";
	const char data5[] = "fedcb";
	hashtableInsert(table, (void*) key1, sizeof(key1), (void*) data1, sizeof(data1));
	hashtableInsert(table, (void*) key2, sizeof(key2), (void*) data2, sizeof(data2));
	hashtableInsert(table, (void*) key3, sizeof(key3), (void*) data3, sizeof(data3));
	hashtableInsert(table, (void*) key4, sizeof(key4), (void*) data4, sizeof(data4));
	hashtableInsert(table, (void*) key5, sizeof(key5), (void*) data5, sizeof(data5));
	hashtableInsert(table, (void*) key6, sizeof(key6), (void*) NULL, sizeof(NULL));
	hashtableDeleteNode(table, (void*) key3);
	hashtableDeleteNode(table, (void*) key3);
	hashtableDeleteNode(table, (void*) key5);
	hashtablePrint(table);
	hashtableFree(table);
} */