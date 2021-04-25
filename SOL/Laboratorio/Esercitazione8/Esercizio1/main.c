#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <utilities.h>

#define MAX_RAND 2048

struct node
{
	void* data;
	struct node* next;
};

struct queue
{
	size_t data_size;
	struct node* first_node;
	struct node* last_node;
};

char isNumber(const char*, long*);
void Pthread_create(pthread_t*, const pthread_attr_t*, void* (*start_routine) (void*), void*);
void Pthread_mutex_lock(pthread_mutex_t*);
void Pthread_mutex_unlock(pthread_mutex_t*);



struct queue* create_queue(size_t);
bool queue_is_empty(const struct queue*);
void push(struct queue**, void*);
void* pop(struct queue*);
void free_queue(struct queue*);

void create_product(char**);
static void* producer(void*);
static void* consumer(void*);

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition = PTHREAD_COND_INITIALIZER;
size_t product_no = 0;
unsigned int* seedp;
struct queue** buffer;
bool empty = true;

int main(int argc, char* argv[])
{
	if (argc != 4)
		goto error_message;

	int err;

	size_t M; // producers
	size_t N; // consumers
	size_t K; // products

	long tmp;
	if (isNumber(argv[1], &tmp) != 0)
		goto error_message;
	M = tmp;
	if (isNumber(argv[2], &tmp) != 0)
		goto error_message;
	N = tmp;
	if (isNumber(argv[3], &tmp) != 0)
		goto error_message;
	K = tmp;


	pthread_t* P;
	pthread_t* C;
	struct queue* _stacked_buffer;
	EXIT_IF_NULL(P, (pthread_t*) malloc(sizeof(pthread_t) * M), "malloc");
	EXIT_IF_NULL(C, (pthread_t*) malloc(sizeof(pthread_t) * N), "malloc");
	_stacked_buffer = create_queue(sizeof(char) * 5); // ((ceil(log10(1024))+1)
	buffer = &_stacked_buffer;
	unsigned int seed = time(NULL);
	seedp = &seed;

	size_t args[2] = { K, M };
	for (size_t i = 0; i < M; i++)
		Pthread_create(&(P[i]), NULL, &producer, (void*) args);
	for (size_t i = 0; i < N; i++)
		Pthread_create(&(C[i]), NULL, &consumer, (void*) &K);

	for (size_t i = 0; i < M; i++)
		EXIT_IF_NOT_EXPECTED_VALUE(err, 0, pthread_join(P[i], NULL), "pthread_join");
	for (size_t i = 0; i < M; i++)
		EXIT_IF_NOT_EXPECTED_VALUE(err, 0, pthread_join(C[i], NULL), "pthread_join");

	free_queue(*buffer);
	free(P);
	free(C);
	return 0;

	error_message:
	fprintf(stderr, "Usage: %s <#producers> <#consumers> <#messages>\n", argv[0]);
	fprintf(stderr, "All the parameters must be positive integers.\n");
	return 1;
}

struct queue* create_queue(size_t dim)
{
	struct node* head;
	EXIT_IF_NULL(head, (struct node*) malloc(sizeof(struct node)), "malloc");

	head->data = NULL;
	head->next = NULL;

	struct queue* q;
	EXIT_IF_NULL(q, (struct queue*) malloc(sizeof(struct queue)), "malloc");
	q->data_size = dim;
	q->first_node = head;
	q->last_node = head;

	return q;
}

bool queue_is_empty(const struct queue* q)
{
	return q->first_node == q->last_node;
}

void push(struct queue** q, void* data)
{
	struct node* new_node;
	EXIT_IF_NULL(new_node, (struct node*) malloc(sizeof(struct node)), "malloc");
	EXIT_IF_NULL(new_node->data, (void*) malloc((*q)->data_size), "malloc");
	memcpy(new_node->data, data, (*q)->data_size);
	new_node->next = NULL;

	if ((*q)->last_node == (*q)->first_node) // queue is empty
	{
		(*q)->first_node->next = new_node;
		(*q)->last_node = new_node;
	}
	else
	{
		(*q)->last_node->next = new_node;
		(*q)->last_node = new_node;
	}
}

void* pop(struct queue* q)
{
	if (q->first_node == q->last_node) // queue is empty
		return NULL;

	struct node* tmp = q->first_node->next;
	if (tmp->next != NULL)
		q->first_node->next = tmp->next;
	else
	{
		q->first_node->next = NULL;
		q->last_node = q->first_node;
	}

	void* data;
	EXIT_IF_NULL(data, (void*) malloc(q->data_size), "malloc");
	memcpy(data, tmp->data, q->data_size);

	free(tmp->data);
	free(tmp); // freeing popped node

	return data;
}

void free_queue(struct queue* q)
{
	if (!q)
		return;

	while (q->first_node->next != NULL)
	{
		struct node* tmp = q->first_node->next;
		q->first_node->next = (q->first_node->next)->next;

		free(tmp->data);
		free(tmp);
	}

	free(q->first_node);
	free(q);
}

char isNumber(const char* s, long* n)
{
	if (s == NULL) return 1;
	if (strlen(s) == 0) return 1;
	char* e = NULL;
	errno = 0;
	long val = strtol(s, &e, 10);
	if (errno == ERANGE) return 2; // overflow/underflow
	if (e != NULL && *e == (char)0)
	{
					*n = val;
					return 0; // successo 
	}
	return 1; // non e' un numero
}

void Pthread_create(pthread_t* thread, const pthread_attr_t* attr, void* (*start_routine) (void*), void* arg)
{
	int err;
	if (( err = pthread_create(thread, attr, start_routine, arg)) != 0)
	{
		errno = err;
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}
}

void Pthread_mutex_lock(pthread_mutex_t *mtx)
{
	int err;
	if (( err = pthread_mutex_lock(mtx) ) != 0)
	{
		errno = err;
		perror("lock");
		pthread_exit((void*) (long) errno);
	}
}

void Pthread_mutex_unlock(pthread_mutex_t *mtx)
{
	int err;
	if (( err = pthread_mutex_unlock(mtx) ) != 0)
	{
		errno = err;
		perror("unlock");
		pthread_exit((void*) (long) errno);
	}
}

void create_product(char** bufferptr)
{
	int x = rand_r(seedp) % MAX_RAND;
	x = (x < 0) ? x + MAX_RAND : x;
	int length = snprintf(NULL, 0, "%d", x);
	char* str;
	EXIT_IF_NULL(str, (char*) malloc((length + 3) * sizeof(char)), perror("malloc"));
	snprintf(str, length + 1, "%d", x);
	*bufferptr = str;
}

static void* producer(void* arg)
{
	size_t* args = (size_t*) arg;
	size_t local_K = args[0];
	size_t local_M = args[1];

	bool no_remainder = (local_K % local_M) == 0;
	size_t aux = 0;
	size_t split_messages = (no_remainder) ? local_K / local_M : 0;
	while(true)
	{
		Pthread_mutex_lock(&mutex);
		if (product_no >= local_K || (no_remainder && (aux == split_messages)))
		{
			fprintf(stdout, "Producer %d has done its job and produced %lu/%lu.\n", (int) pthread_self(), aux, product_no);
			fflush(stdout);
			pthread_cond_signal(&condition);
			Pthread_mutex_unlock(&mutex);
			break;
		}
		char* tmp;
		create_product(&tmp);
		product_no++;
		aux++;
		fprintf(stdout, "Product (no. %lu/%lu) %s has been created.\n", product_no, local_K, tmp);
		fflush(stdout);
		push(buffer, (void*) tmp);
		if (empty)
		{
			empty = false;
			pthread_cond_signal(&condition);
		}
		Pthread_mutex_unlock(&mutex);
		free(tmp);
	}

	return (void*) 0;
}

static void* consumer(void* arg)
{
	size_t* tmp = (size_t*) arg;
	size_t local_K = *tmp;
	while(true)
	{
		Pthread_mutex_lock(&mutex);
		while(queue_is_empty(*buffer))
		{
			if (product_no >= local_K && queue_is_empty(*buffer))
			{
				pthread_cond_broadcast(&condition);
				Pthread_mutex_unlock(&mutex);
				fprintf(stdout, "Consumer %d has done its job!\n", (int) pthread_self());
				fflush(stdout);
				return (void*) 0;
			}
			fprintf(stdout, "Consumer %d waiting for buffer to be refilled : %lu/%lu items have been consumed.\n", (int) pthread_self(), product_no, local_K);
			fflush(stdout);
			pthread_cond_wait(&condition, &mutex);
			fprintf(stdout, "Consumer %d has been woken up!\n", (int) pthread_self());
			fflush(stdout);
		}
		char* str = (char*) pop(*buffer);
		if (str == NULL)
		{
			fprintf(stderr, "Consumer %d successfully popped NULL item from buffer.\n", (int) pthread_self());
			exit(EXIT_FAILURE);
		}
		fprintf(stdout, "Product %s has been consumed.\n", str);
		fflush(stdout);

		if (queue_is_empty(*buffer)) empty = true;
		Pthread_mutex_unlock(&mutex);

		free(str);
	}

	return (void*) 0;
}