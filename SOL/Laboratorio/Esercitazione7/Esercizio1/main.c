#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <wrappers.h>

int buffer[1];
bool empty = true;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int produci();
int estrai();
static void* produttore(void*);
static void* consumatore(void*);

int main(int argc, char* argv[])
{
	srand(time(0));
	pthread_t P, C;
	int status1, status2;
	int err;

	Pthread_create(&P, NULL, &produttore, NULL);
	Pthread_create(&C, NULL, &consumatore, NULL);

	sleep(10);
	if (( err = pthread_cancel(P) ) != 0) 
	{
		errno = err;
		perror("pthread_cancel");
		exit(EXIT_FAILURE);
	}
	// fprintf(stdout, "P canceled.\n");
	if (( err = pthread_join(P, (void*) &status1) ) != 0)
	{
		errno = err;
		perror("pthread_join");
		exit(EXIT_FAILURE);
	}
	// fprintf(stdout, "P joined.\n");

	if (( err = pthread_cancel(C) ) != 0) 
	{
		errno = err;
		perror("pthread_cancel");
		exit(EXIT_FAILURE);
	}
	// fprintf(stdout, "C canceled.\n");
	if (( err = pthread_join(C, (void*) &status2) ) != 0)
	{
		errno = err;
		perror("pthread_join");
		exit(EXIT_FAILURE);
	}
	// fprintf(stdout, "P joined.\n");

	exit(EXIT_SUCCESS);
}

int produci()
{
	empty = false;
	return rand();
}

int estrai()
{
	empty = true;
	return buffer[0];
}

static void* produttore(void* argomento_ignorato)
{
	while (true)
	{
		Pthread_mutex_lock(&mtx);
		while (!empty)
		{
			pthread_cond_wait(&cond, &mtx);
			// fprintf(stdout, "Producer has woken up!\n");
			// fflush(stdout);
		}
		buffer[0] = produci();
		empty = false;
		// fprintf(stdout, "Product %d has been created.\n", buffer[0]);
		// fflush(stdout);
		pthread_cond_signal(&cond);
		Pthread_mutex_unlock(&mtx);
		// fprintf(stdout, "Producer testing for cancel.\n");
		pthread_testcancel();
		// fprintf(stdout, "Producer has not been canceled.\n");
	}

	return (void*) 0;
}

static void* consumatore(void* argomento_ignorato)
{
	while (true)
	{
		// fprintf(stdout, "Consumer acquiring lock...\n");
		// fflush(stdout);
		Pthread_mutex_lock(&mtx);
		// fprintf(stdout, "Consumer acquired lock.\n");
		// fflush(stdout);
		while (empty)
		{
			// fprintf(stdout, "Consumer waiting on cond...\n");
			// fflush(stdout);
			pthread_cond_wait(&cond, &mtx);
			// fprintf(stdout, "Consumer has woken up!\n");
			// fflush(stdout);
		}
		// fprintf(stdout, "Product %d has been consumed.\n", buffer[0]);
		empty = true;
		pthread_cond_signal(&cond);
		Pthread_mutex_unlock(&mtx);
		pthread_testcancel();
	}

	return (void*) 0;
}