#include <wrappers.h>


void Pthread_create(pthread_t* thread, const pthread_attr_t* attr, void* (*start_routine) (void*), void* arg)
{
	int err;
	if (( err = pthread_create(thread, attr, start_routine, arg)) != 0)
	{
		errno = err;
		perror("pthread_create");
		exit(1);
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