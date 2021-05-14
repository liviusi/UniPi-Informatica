#define _POSIX_C_SOURCE 199506L

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
#define SOCKETMAXQUEUESIZE 5
#define SOCKETNAME "Esercizio3/socket"
#define EXITCOMMAND "exit"
#define SIGNAL_NOT_RECEIVED false
#define SIGNAL_RECEIVED true

static void* capitalize(void*);
static void* signal_handler_func(void*);
void cleanup_server(int);
void spawn_thread(int, int);

pthread_t thread[SOCKETMAXQUEUESIZE];
bool completed[SOCKETMAXQUEUESIZE];
int initialized = 0;
int server_fd;
struct sigaction s;
bool flag = SIGNAL_NOT_RECEIVED;

int main(void)
{
	pthread_t signal_handler_thread;
	int err;
	sigset_t mask;
	EXIT_IF_EQ(err, -1, sigemptyset(&mask), sigemptyset);
	EXIT_IF_EQ(err, -1, sigaddset(&mask, SIGINT), sigaddset);
	EXIT_IF_EQ(err, -1, sigaddset(&mask, SIGQUIT), sigaddset);
	EXIT_IF_EQ(err, -1, sigaddset(&mask, SIGTERM), sigaddset);
	EXIT_IF_EQ(err, -1, pthread_sigmask(SIG_BLOCK, &mask, NULL), pthread_sigmask);
	EXIT_IF_NEQ(err, 0, pthread_create(&signal_handler_thread, NULL, signal_handler_func, 
			(void*) &mask), pthread_create);

	struct sigaction s;
    memset(&s, 0, sizeof(s));    
    s.sa_handler=SIG_IGN;
    EXIT_IF_EQ(err, -1, sigaction(SIGPIPE, &s, NULL), sigaction);

	int client_fd;
	struct sockaddr_un socket_address;
	for (int i = 0; i < SOCKETMAXQUEUESIZE; i++) completed[i] = true;

	strncpy(socket_address.sun_path, SOCKETNAME, UNIX_PATH_MAX);
	socket_address.sun_family = AF_UNIX;
	EXIT_IF_EQ(server_fd, -1, socket(AF_UNIX, SOCK_STREAM, 0), socket);
	EXIT_IF_EQ(err, -1, bind(server_fd, (struct sockaddr*) &socket_address, 
			sizeof(socket_address)), bind);
	fprintf(stdout, "[SERVER] listening\n");
	EXIT_IF_EQ(err, -1, listen(server_fd, SOCKETMAXQUEUESIZE), listen);

	while (true)
	{
		client_fd = accept(server_fd, NULL, 0);
		if (client_fd == -1)
		{
			if (errno == EINVAL && flag == SIGNAL_RECEIVED) break;
			else
			{
				perror("accept");
				exit(EXIT_FAILURE);
			}
		}

		fprintf(stdout, "[SERVER] accepted new client %d\n", client_fd);
		fflush(stdout);

		for (int j = 0; j < SOCKETMAXQUEUESIZE; j++)
		{
			if (completed[j])
			{
				if (pthread_equal(pthread_self(), thread[j]) == 0)
					EXIT_IF_EQ(err, -1, pthread_join(thread[j], NULL), pthread_join);
				spawn_thread(client_fd, j);
				break;
			}
		}
		initialized++;
	}

	cleanup_server(initialized);
	return 1;
}

static void* signal_handler_func(void* arg)
{
	sigset_t* mask = (sigset_t*) arg;
	int err, signal_caught;

	EXIT_IF_EQ(err, -1, pthread_detach(pthread_self()), pthread_detach);
	while (true)
	{
		EXIT_IF_NEQ(err, 0, sigwait(mask, &signal_caught), sigwait);
		fprintf(stderr, "\n[SIGNAL] Caught signal %d\n", signal_caught);

		switch (signal_caught)
		{
			case SIGINT:
			case SIGQUIT:
			case SIGTERM:
				flag = SIGNAL_RECEIVED;
				shutdown(server_fd, SHUT_RDWR);
				return (void*) 0;
			default:
				break;
		}
	}
	exit(EXIT_FAILURE);
}

static void* capitalize(void* arg)
{
	int* tmp = (int*) arg;
	int client_fd = tmp[0];
	int pos = tmp[1];
	free(arg);
	char message[BUFFERSIZE];
	ssize_t n;
	int err;

	while(true)
	{
		if (flag == SIGNAL_RECEIVED) break;
		EXIT_IF_EQ(n, -1, read(client_fd, message, BUFFERSIZE), read);
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

	completed[pos] = true;
	EXIT_IF_EQ(err, -1, close(client_fd), "close");
	return (void*) 0;

}

void spawn_thread(int client_fd, int pos)
{
	int err;
	sigset_t mask, old_mask;
	sigemptyset(&mask);
	sigaddset(&mask, SIGINT);
	sigaddset(&mask, SIGPIPE);
	sigaddset(&mask, SIGHUP);
	sigaddset(&mask, SIGTERM);
	sigaddset(&mask, SIGQUIT);
	EXIT_IF_EQ(err, -1, pthread_sigmask(SIG_BLOCK, &mask, &old_mask), pthread_sigmask);

	pthread_t worker;
	int* arg;
	EXIT_IF_NULL(arg, (int*) malloc(sizeof(int) * 2), malloc);
	arg[0] = client_fd;
	arg[1] = pos;
	completed[pos] = false;
	EXIT_IF_NEQ(err, 0, pthread_create(&worker, NULL, capitalize, (void*) arg), pthread_create);
	thread[pos] = worker;
	EXIT_IF_NEQ(err, 0, pthread_sigmask(SIG_SETMASK, &old_mask, NULL), pthread_sigmask);
	return;
}

void cleanup_server(int initialized)
{
	int err;
	EXIT_IF_EQ(err, -1, close(server_fd), close);
	EXIT_IF_EQ(err, -1, remove(SOCKETNAME), remove);
	for (int i = 0; i < initialized; i++)
	{
		if (completed[i] == false)
			EXIT_IF_NEQ(err, 0, pthread_join(thread[i], NULL), pthread_join);
	}
	exit(EXIT_SUCCESS);
}