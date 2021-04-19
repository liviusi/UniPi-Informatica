#define _POSIX_C_SOURCE 199506L

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <utilities.h>

#define MAX_SLEEP 100000000

// custom types
// used to define philosophers' possible activities
enum activity
{
	IS_THINKING,
	IS_HUNGRY,
	IS_EATING
};

static void* philosopher_routine(void*);
size_t modulo(int, size_t);

// global variables
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t** condition;
enum activity** philosopher_current_activity;
unsigned int* N;

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <N>\n", argv[0]);
		return 1;
	}

	if (atoi(argv[1]) < 5)
	{
		fprintf(stderr, "N is not an integer >= 5.\n");
		return 1;
	}
	unsigned int thread_num = atoi(argv[1]);
	N = &thread_num;

	int err;

	pthread_t* philosopher;
	pthread_cond_t* _stacked_condition;
	enum activity* _stacked_philosopher_current_activity;
	EXIT_IF_NULL(philosopher, (pthread_t*) malloc(sizeof(pthread_t) * (*N)), "malloc");
	EXIT_IF_NULL(_stacked_condition, (pthread_cond_t*) malloc(sizeof(pthread_cond_t) * (*N)),
			"malloc");
	EXIT_IF_NULL((_stacked_philosopher_current_activity), (enum activity*)
			malloc(sizeof(enum activity) * (*N)), "malloc");
	
	condition = &(_stacked_condition);
	philosopher_current_activity = &(_stacked_philosopher_current_activity);
	
	// at first every philosopher is thinking
	for (size_t i = 0; i < *N; i++)
		(*philosopher_current_activity)[i] = IS_THINKING;

	for (size_t i = 0; i < *N; i++)
		EXIT_IF_NOT_EXPECTED_VALUE(err, 0, pthread_cond_init(&((*condition)[i]), NULL),
			"pthread_cond_init");

	for (size_t i = 0; i < *N; i++)
	{
		size_t* pos;
		EXIT_IF_NULL(pos, (size_t*) malloc(sizeof(*pos)), "malloc");
		*pos = i;
		Pthread_create(&(philosopher[i]), NULL, &philosopher_routine, (void*) pos);
	}


	for (size_t i = 0; i < *N; i++)
	{
		// EXIT_IF_NOT_EXPECTED_VALUE(err, 0, pthread_cancel(philosopher[i]), "pthread_cancel");
		// fprintf(stdout, "(thread) philosopher[%lu] has been canceled successfully.\n", i);
		// fflush(stdout);
		EXIT_IF_NOT_EXPECTED_VALUE(err, 0, pthread_join(philosopher[i], NULL), "pthread_join");
		fprintf(stdout, "(thread) philosopher[%lu] has joined successfully.\n", i);
		fflush(stdout);
	}

	free(*condition);
	free(philosopher);

	return 0;
}

size_t modulo(int a, size_t mod)
{
	if (a < 0) return (size_t) (a + mod);
	else return (size_t) (a % mod);
}

static void* philosopher_routine(void* arg)
{
	// pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
	size_t i = 0;
	int err;
	size_t position = *((size_t*) arg);
	free(arg);
	unsigned int seed = (unsigned int) time(NULL);
	// struct thread_stats status = {0, 0};
	struct timespec delay;
	
	while (true) // it needs to complete this cycle 100 times
	{
		// THINKING
		Pthread_mutex_lock(&mutex);
		(*philosopher_current_activity)[position] = IS_THINKING;
		fprintf(stdout, "%lu: philosopher[%lu] is thinking.\n", i, position);
		fflush(stdout);

		// waking up neighbours
		if ((*philosopher_current_activity)[modulo(((int) position) - 1, (*N))] == IS_HUNGRY &&
				(*philosopher_current_activity)[modulo(((int) position) - 2, (*N))] != IS_EATING)
		{
			(*philosopher_current_activity)[modulo(((int) position) - 1, (*N))] = IS_EATING;
			fprintf(stdout, "Waking up philosopher[%lu]\n", modulo(((int) position) - 1, (*N)));
			fflush(stdout);
			pthread_cond_signal(&((*condition)[modulo(((int) position) - 1, (*N))]));
		}
		if ((*philosopher_current_activity)[modulo((int) position + 1, (*N))] == IS_HUNGRY &&
				(*philosopher_current_activity)[modulo((int) position + 2, (*N))] != IS_EATING)
		{
			(*philosopher_current_activity)[modulo((int) position + 1, (*N))] = IS_EATING;
			fprintf(stdout, "Waking up philosopher[%lu]\n", modulo(((int) position) + 1, (*N)));
			fflush(stdout);
			pthread_cond_signal(&((*condition)[modulo((int) position + 1, (*N))]));
		}

		// checking if the cycle has been executed 300 times
		Pthread_mutex_unlock(&mutex);

		if (i == 300) break;

		// else: sleep
		delay.tv_sec = 0;
		delay.tv_nsec = abs( rand_r(&seed) % MAX_SLEEP );
		EXIT_IF_NOT_EXPECTED_VALUE(err, 0, nanosleep(&delay, NULL), "nanosleep");

		// wants to eat
		Pthread_mutex_lock(&mutex);
		(*philosopher_current_activity)[position] = IS_HUNGRY;
		fprintf(stdout, "%lu: philosopher[%lu] is hungry.\n", i, position);
		fflush(stdout);
		while ( (*philosopher_current_activity)[modulo(((int) position) - 1, (*N))] == IS_EATING
				|| (*philosopher_current_activity)[modulo((int) position + 1, (*N))] == IS_EATING )
		{
			fprintf(stdout, "philosopher[%lu] is waiting on their neighbours %lu and %lu.\n", position, modulo(((int) position) - 1, (*N)), modulo((int) position + 1, (*N)));
			fflush(stdout);
			pthread_cond_wait(&((*condition)[position]), &mutex);
		}

		// start eating
		(*philosopher_current_activity)[position] = IS_EATING;
		fprintf(stdout, "%lu: philosopher[%lu] is eating.\n", i, position);
		fflush(stdout);
		Pthread_mutex_unlock(&mutex);

		// sleeping
		delay.tv_sec = 0;
		delay.tv_nsec = abs( rand_r(&seed) % MAX_SLEEP );
		EXIT_IF_NOT_EXPECTED_VALUE(err, 0, nanosleep(&delay, NULL), "nanosleep");
		fprintf(stdout, "philosopher[%lu] has completed cycle no. %lu\n", position, i + 1);
		fflush(stdout);
		i++;
	}
	fprintf(stdout, "philosopher[%lu] has completed their dinner.\n", position);
	fflush(stdout);
	// pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_exit((void*) 0);
}