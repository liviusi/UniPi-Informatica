#define _POSIX_C_SOURCE 200112L

#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <ctype.h>
#include <pthread.h>
#include <signal.h>
#include <utilities.h>

#define BUFFERSIZE 64
#define UNIX_PATH_MAX 108
#define SOCKETMAXQUEUESIZE 32
#define SOCKETNAME "Esercizio1/socket"
#define EXITCOMMAND "exit"
#define SIGNAL_RECEIVED 1
#define SIGNAL_NOT_RECEIVED 0

typedef struct args
{
	int pos;
	int client_fd;
} args_t;

typedef struct threads_info
{
	pthread_t tid;
	bool completed;
} thread_info_t;

static void handler(int);
void spawn_thread(int, int);
static void* capitalize(void*);
void cleanup_server(int);
char string_to_int(const char*, int*);

struct sigaction s;
volatile sig_atomic_t flag = SIGNAL_NOT_RECEIVED;
thread_info_t* threads;
int N, i = 0;

int main(int argc, char* argv[])
{
	if (argc != 2 || string_to_int(argv[1], &N) != 0)
	{
		fprintf(stderr, "Usage %s N\nN must be a positive integer.\n", argv[0]);
		return 1;
	}

	int err;
	sigset_t mask, old_mask;
	sigemptyset(&mask);
	sigaddset(&mask, SIGINT);
	sigaddset(&mask, SIGQUIT);

	EXIT_IF_NEQ(err, 0, pthread_sigmask(SIG_BLOCK, &mask, &old_mask), pthread_sigmask);

	int server_fd;
	EXIT_IF_NULL(threads, (thread_info_t*) malloc(sizeof(thread_info_t) * N), malloc);
	for (int i = 0; i < N; i++)
	{
		threads[i].tid = pthread_self();
		threads[i].completed = false;
	}
	struct sockaddr_un socket_address;

	memset(&s, 0, sizeof(s));
	s.sa_handler = handler;
	EXIT_IF_EQ(err, -1, sigaction(SIGINT, &s, NULL), sigaction);
	EXIT_IF_EQ(err, -1, sigaction(SIGQUIT, &s, NULL), sigaction);
	strncpy(socket_address.sun_path, SOCKETNAME, UNIX_PATH_MAX);
	socket_address.sun_family = AF_UNIX;
	EXIT_IF_EQ(server_fd, -1, socket(AF_UNIX, SOCK_STREAM, 0), socket);
	EXIT_IF_EQ(err, -1, bind(server_fd, (struct sockaddr*) &socket_address, 
			sizeof(socket_address)), bind);
	fprintf(stdout, "[SERVER] listening\n");
	EXIT_IF_EQ(err, -1, listen(server_fd, SOCKETMAXQUEUESIZE), listen);
	EXIT_IF_NEQ(err, 0, pthread_sigmask(SIG_SETMASK, &old_mask, NULL), pthread_sigmask);

	while (flag == SIGNAL_NOT_RECEIVED)
	{
		while (i < N)
		{
			fprintf(stdout, "[DEBUG] threads[%d].tid = %lu, pthread_self = %lu\n", i, threads[i].tid, pthread_self());
			if (pthread_equal(threads[i].tid, pthread_self()) != 0) // equals
			{
				int client_fd = accept(server_fd, NULL, 0);
				if (errno == EINTR) break;
				fprintf(stdout, "[SERVER] accepted new client %d\n", client_fd);
				fflush(stdout);
				spawn_thread(client_fd, i);
			}
			i++;
		}
		for (int j = 0; j < N; j++)
		{
			// bool freed = false;
			if (threads[j].completed == true && pthread_equal(threads[j].tid, pthread_self()) == 0)
			{
				fprintf(stdout, "[SERVER] Joined with worker %lu\n", threads[j].tid);
				threads[j].completed = false;
				EXIT_IF_NEQ(err, 0, pthread_join(threads[j].tid, NULL), pthread_join);
				threads[j].tid = pthread_self();
				// freed = true;
			}
			// if (freed) fprintf(stderr, "[SERVER] Clients can now be accepted again.\n");
		}
	}
	cleanup_server(server_fd);
}

void cleanup_server(int server_fd)
{
	int err;
	for (int i = 0; i < N; i++)
	{
		if (pthread_equal(threads[i].tid, pthread_self()) == 0)
		{
			if (threads[i].completed == true) continue;
			EXIT_IF_NEQ(err, 0, pthread_join(threads[i].tid, NULL), pthread_join);
			fprintf(stdout, "[SERVER] Joined with worker %lu\n", threads[i].tid);
		}
	}
	EXIT_IF_EQ(err, -1, close(server_fd), close);
	EXIT_IF_EQ(err, -1, remove(SOCKETNAME), remove);
	free(threads);
	exit(EXIT_SUCCESS);
}

void spawn_thread(int client_fd, int i)
{
	int err;
	sigset_t mask, old_mask;
	sigemptyset(&mask);
	sigaddset(&mask, SIGINT);
	sigaddset(&mask, SIGQUIT);
	EXIT_IF_EQ(err, -1, pthread_sigmask(SIG_BLOCK, &mask, &old_mask), pthread_sigmask);

	pthread_t worker;
	args_t* arg;
	EXIT_IF_NULL(arg, (args_t*) malloc(sizeof(args_t)), malloc);
	arg->client_fd = client_fd;
	arg->pos = i;
	EXIT_IF_NEQ(err, 0, pthread_create(&worker, NULL, capitalize, (void*) arg), pthread_create);
	threads[i].completed = false;
	threads[i].tid = worker;
	fprintf(stdout, "[SERVER] Created worker %lu\n", threads[i].tid);
	EXIT_IF_NEQ(err, 0, pthread_sigmask(SIG_SETMASK, &old_mask, NULL), pthread_sigmask);
	return;
}

static void* capitalize(void* arg)
{
	args_t* tmp = (args_t*) arg;
	int client_fd = tmp->client_fd;
	int pos = tmp->pos;
	free(arg);

	int err;
	char message[BUFFERSIZE];
	ssize_t n;

	while(true)
	{
		if (flag == SIGNAL_RECEIVED) break;
		n = read(client_fd, message, BUFFERSIZE);
		if (n == -1)
		{
			if (errno == EINTR) break;
			else
			{
				perror("read");
				exit(EXIT_FAILURE);
			}
		}
		message[BUFFERSIZE - 1] = '\0';
		fprintf(stdout, "[SERVER] Read from client %d: %s", client_fd, message);

		if(strncmp(message, EXITCOMMAND, strlen(EXITCOMMAND)) != 0)
		{
			for (size_t i = 0; message[i] != '\0'; i++)
				message[i] = toupper(message[i]);
			fprintf(stdout, "[SERVER] Writing to client %d: %s", client_fd, message);
			EXIT_IF_EQ(n, -1, write(client_fd, message, BUFFERSIZE), write);
		}
		else break;
	}

	i--;
	threads[pos].completed = true;
	EXIT_IF_EQ(err, -1, close(client_fd), "close");
	return (void*) 0;
}

static void handler(int signum)
{
	flag = SIGNAL_RECEIVED;
	static const char flag_is_set[] = "\nFlag has been set!\n";
	write(STDOUT_FILENO, flag_is_set, sizeof(flag_is_set));
}

char string_to_int(const char* s, int* n)
{
	if (s == NULL) return 1;
	if (strlen(s) == 0) return 1;
	char* e = NULL;
	errno = 0;
	int val = (int) strtol(s, &e, 10);
	if (errno == ERANGE) return 2; // overflow/underflow
	if (e != NULL && *e == (char)0)
	{
			if (val < 0) return -1; // non valido
			*n = val;
			return 0; // successo 
	}
	return 1; // non e' un numero
}