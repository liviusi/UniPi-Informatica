#define _POSIX_C_SOURCE 200112L

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <icl_hash.h>
#include <unboundedfifo.h>
#include <wrappers.h>

#define N 2
#define BUCKETS 512
#define MAXTOKEN 128
#define MAXBUFFER 1024

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
// condition[0] -> reading line from file
// condition[1] -> tokenizing line from buffer
pthread_cond_t** condition;

static void* reading_routine(void*);
static void* tokenizing_routine(void*);
static void* printing_routine(void*);

struct queue** buffer_queue;
struct queue** token_queue;
bool reading_is_done = false;
bool tokenizing_is_done = false;
bool buffer_empty = true;
bool token_empty = true;

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <input filename>\n", argv[0]);
		return 1;
	}

	int err;
	pthread_t reading;
	pthread_t tokenizing;
	pthread_t printing;
	pthread_cond_t* _stacked_condition;
	EXIT_IF_NULL(_stacked_condition, (pthread_cond_t*) malloc(sizeof(pthread_cond_t) * N), "malloc");
	condition = &(_stacked_condition);
	struct queue* _stacked_buffer_queue;
	struct queue* _stacked_token_queue;
	_stacked_buffer_queue = create_queue(sizeof(char) * MAXBUFFER);
	buffer_queue = &_stacked_buffer_queue;
	_stacked_token_queue = create_queue(sizeof(char) * MAXTOKEN);
	token_queue = &_stacked_token_queue;

	for (size_t i = 0; i < N; i++)
		EXIT_IF_NOT_EXPECTED_VALUE(err, 0, pthread_cond_init(&((*condition)[i]), NULL),
			"pthread_cond_init");
	
	FILE* fd;
	FOPEN(fd, argv[1], "r");

	Pthread_create(&reading, NULL, &reading_routine, (void*) fd);
	Pthread_create(&tokenizing, NULL, &tokenizing_routine, NULL);
	Pthread_create(&printing, NULL, &printing_routine, NULL);

	EXIT_IF_NOT_EXPECTED_VALUE(err, 0, pthread_join(reading, NULL), "pthread_join");
	EXIT_IF_NOT_EXPECTED_VALUE(err, 0, pthread_join(tokenizing, NULL), "pthread_join");
	EXIT_IF_NOT_EXPECTED_VALUE(err, 0, pthread_join(printing, NULL), "pthread_join");


	fclose(fd);
	free(*condition);
	free_queue(*buffer_queue);
	free_queue(*token_queue);
	return 0;
}

static void* reading_routine(void* arg)
{
	FILE* fd = (FILE*) arg;
	char* buffer;
	EXIT_IF_NULL(buffer, (char*) calloc(MAXBUFFER, sizeof(char)), "calloc");
	char* dummy_buffer = buffer;

	while (true)
	{
		while (fgets(buffer, MAXBUFFER, fd))
		{
			Pthread_mutex_lock(&mutex);
			//fprintf(stdout, "Pushing\n%son buffer_queue\n", buffer);
			//fflush(stdout);
			push(buffer_queue, (void*)buffer);
			if (buffer_empty)
			{
				buffer_empty = false;
				pthread_cond_signal(&((*condition)[0]));
			}
			Pthread_mutex_unlock(&mutex);
		}
		break;
	}

	// buffer is now null so it can't be freed
	// I need to call free on dummy_buffer
	free(dummy_buffer);
	reading_is_done = true;
	return (void*) 0;
}

static void* tokenizing_routine(void* arg)
{
	char* buffer;
	char* tmp_buffer;
	char* saveptr;
	char* token;

	while(true)
	{
		if (buffer_empty && reading_is_done) break;
		Pthread_mutex_lock(&mutex);
		if (buffer_empty)
		{
			//fprintf(stdout, "buffer_queue is empty: waiting on condition[0]...\n");
			//fflush(stdout);
			pthread_cond_wait(&(*(condition)[0]), &mutex);
			//fprintf(stdout, "buffer_queue is not empty anymore. Resuming tokenizing_routine.\n");
			//fflush(stdout);
		}
		buffer = (char*) pop(*buffer_queue);
		//fprintf(stdout, "Popped\n%sfrom buffer_queue\n", buffer);
		//fflush(stdout);
		if (queue_is_empty(*buffer_queue)) buffer_empty = true;
		Pthread_mutex_unlock(&mutex);

		tmp_buffer = buffer;
		token = strtok_r(buffer, " ", &saveptr);
		while(token != NULL)
		{
			Pthread_mutex_lock(&mutex);
			//fprintf(stdout, "Pushing %s to token_queue...\n", token);
			//fflush(stdout);
			token[strcspn(token, "\n")] = 0; // delete trailing newline
			if (token)
			{
				push(token_queue, (void*) token);
				if (token_empty)
				{
					token_empty = false;
					pthread_cond_signal(&((*condition)[1]));
				}
				Pthread_mutex_unlock(&mutex);
			}
			token = strtok_r(NULL, " ", &saveptr);
		}

		free(tmp_buffer);
	}

	tokenizing_is_done = true;
	return (void*) 0;
}

static void* printing_routine(void* arg)
{
	icl_hash_t* table = icl_hash_create(BUCKETS, NULL, NULL);
	if (!table)
	{
		fprintf(stderr, "icl_hash_create has failed.\n");
		exit(EXIT_FAILURE);
	}
	bool added_one = false;
	while (true)
	{
		if (token_empty && tokenizing_is_done) break;
		Pthread_mutex_lock(&mutex);
		if (token_empty)
		{
			//fprintf(stdout, "Waiting on condition[1]: buffer to print is currently empty.\n");
			//fflush(stdout);
			pthread_cond_wait(&((*condition)[1]), &mutex);
		}

		char* dummy = (char*) pop(*token_queue);
		if (queue_is_empty(*token_queue)) token_empty = true;
		Pthread_mutex_unlock(&mutex);
		dummy[strcspn(dummy, "\n")] = 0;
		if (dummy && dummy[0] != '\0')
		{
			char* str;
			EXIT_IF_NULL(str, (char*) calloc(MAXTOKEN, sizeof(char)), "calloc");
			size_t str_index = 0;
			for (size_t dummy_index = 0; dummy[dummy_index] != '\0'; dummy_index++)
			{
				dummy[dummy_index] = tolower(dummy[dummy_index]);
				if (!(ispunct(dummy[dummy_index])) && !(isspace(dummy[dummy_index])))
				{
					str[str_index] = dummy[dummy_index];
					str_index++;
				}
			}
			if (!icl_hash_find(table, (void*) str))
			{
				if (!added_one) added_one = true;
				icl_hash_insert(table, str, (void*) str);
			}
			else free(str);
			free(dummy);
		}
	}

	int k;
	icl_entry_t *entry;
	char *key, *value;

	if (added_one)
	{
		fprintf(stdout, "Words found:\n");
		icl_hash_foreach(table, k, entry, key, value)
			fprintf(stdout, "- %s\n", value);
	}
	else
		fprintf(stdout, "Could not add any word.\n");

	icl_hash_destroy(table, NULL, free);
	return (void*) 0;
}