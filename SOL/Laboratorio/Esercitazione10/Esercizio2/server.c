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
#define SOCKETMAXQUEUESIZE 16
#define SOCKETNAME "Esercizio2/socket"
#define EXITCOMMAND "exit"
#define SIGNAL_RECEIVED 1
#define SIGNAL_NOT_RECEIVED 0

static void handler(int);
static void* capitalize(void*);
void cleanup_server(int, int);
void spawn_thread(int, int);

pthread_t thread[SOCKETMAXQUEUESIZE];
bool completed[SOCKETMAXQUEUESIZE];
struct sigaction s;
volatile sig_atomic_t flag = SIGNAL_NOT_RECEIVED;

int main(void)
{
	int err;
	sigset_t mask, old_mask;
	sigemptyset(&mask);
	sigaddset(&mask, SIGINT);
	sigaddset(&mask, SIGQUIT);
	EXIT_IF_NEQ(err, 0, pthread_sigmask(SIG_BLOCK, &mask, &old_mask), pthread_sigmask);

	int server_fd, client_fd;
	for (int i = 0; i < SOCKETMAXQUEUESIZE; i++) completed[i] = true;
	for (int i = 0; i < SOCKETMAXQUEUESIZE; i++) thread[i] = pthread_self();
	int i = 0;
	struct sockaddr_un socket_address;

	memset(&s, 0, sizeof(s));
	s.sa_handler = handler;
	EXIT_IF_EQ(err, -1, sigaction(SIGINT, &s, NULL), sigaction);
	EXIT_IF_EQ(err, -1, sigaction(SIGPIPE, &s, NULL), sigaction);
	EXIT_IF_EQ(err, -1, sigaction(SIGHUP, &s, NULL), sigaction);
	EXIT_IF_EQ(err, -1, sigaction(SIGTERM, &s, NULL), sigaction);
	EXIT_IF_EQ(err, -1, sigaction(SIGQUIT, &s, NULL), sigaction);

	strncpy(socket_address.sun_path, SOCKETNAME, UNIX_PATH_MAX);
	socket_address.sun_family = AF_UNIX;
	EXIT_IF_EQ(server_fd, -1, socket(AF_UNIX, SOCK_STREAM, 0), socket);
	EXIT_IF_EQ(err, -1, bind(server_fd, (struct sockaddr*) &socket_address, 
			sizeof(socket_address)), bind);
	EXIT_IF_EQ(err, -1, listen(server_fd, SOCKETMAXQUEUESIZE), listen);
	EXIT_IF_NEQ(err, 0, pthread_sigmask(SIG_SETMASK, &old_mask, NULL), pthread_sigmask);

	while (flag == SIGNAL_NOT_RECEIVED)
	{
		client_fd = accept(server_fd, NULL, 0);
		if (client_fd == -1)
		{
			if (errno == EINTR) break;
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
		i++;
	}
	cleanup_server(server_fd, i);
	return 1;
}

void cleanup_server(int server_fd, int initialized)
{
	int err;
	EXIT_IF_EQ(err, -1, close(server_fd), close);
	EXIT_IF_EQ(err, -1, remove(SOCKETNAME), remove);
	for (int i = 0; i <= initialized; i++)
		EXIT_IF_EQ(err, -1, pthread_join(thread[i], NULL), pthread_join);
	exit(EXIT_SUCCESS);
}

void spawn_thread(int client_fd, int i)
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
	arg[1] = (int) i;
	completed[i] = false;
	EXIT_IF_NEQ(err, 0, pthread_create(&worker, NULL, capitalize, (void*) arg), pthread_create);
	thread[i] = worker;
	EXIT_IF_NEQ(err, 0, pthread_sigmask(SIG_SETMASK, &old_mask, NULL), pthread_sigmask);
	return;
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
		EXIT_IF_EQ(n, -1, read(client_fd, message, BUFFERSIZE), "read");
		message[BUFFERSIZE - 1] = '\0';
		fprintf(stdout, "[SERVER] Read from client %d: %s", client_fd, message);

		if(strncmp(message, EXITCOMMAND, strlen(EXITCOMMAND)) != 0)
		{
			for (size_t i = 0; message[i] != '\0'; i++)
				message[i] = toupper(message[i]);
			fprintf(stdout, "[SERVER] Writing to client %d: %s", client_fd, message);
			EXIT_IF_EQ(n, -1, write(client_fd, message, BUFFERSIZE), "write");
		}
		else break;
	}
	completed[pos] = true;
	EXIT_IF_EQ(err, -1, close(client_fd), "close");
	return (void*) 0;

}

static void handler(int signum)
{
	flag = SIGNAL_RECEIVED;
}