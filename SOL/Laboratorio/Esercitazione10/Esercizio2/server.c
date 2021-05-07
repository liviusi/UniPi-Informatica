#define _POSIX_C_SOURCE 199309L

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
#define SOCKETNAME "Esercizio2/socket"
#define EXITCOMMAND "exit"
#define SIGNAL_RECEIVED 1
#define SIGNAL_NOT_RECEIVED 0

static void handler(int);
static void* capitalize(void*);
void cleanup_server(int, size_t);

pthread_t thread[SOCKETMAXQUEUESIZE];
struct sigaction s;
volatile sig_atomic_t flag = SIGNAL_NOT_RECEIVED;

int main(void)
{
	int err, server_fd, client_fd;
	size_t i = 0;
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
	EXIT_IF_EQ(server_fd, -1, socket(AF_UNIX, SOCK_STREAM, 0), "socket");
	EXIT_IF_EQ(err, -1, bind(server_fd, (struct sockaddr*) &socket_address, 
			sizeof(socket_address)), "bind");

	while (true)
	{
		while (flag == SIGNAL_NOT_RECEIVED)
		{
			fprintf(stdout, "[SERVER] listening\n");
			err = listen(server_fd, SOCKETMAXQUEUESIZE);
			if (err == -1)
			{
				if (flag == SIGNAL_RECEIVED) cleanup_server(server_fd, i);
				else
				{
					perror("listen");
					exit(EXIT_FAILURE);
				}
			}
			client_fd = accept(server_fd, NULL, 0);
			if (client_fd == -1)
			{
				if (flag == SIGNAL_RECEIVED) cleanup_server(server_fd, i);
				else
				{
					perror("accept");
					exit(EXIT_FAILURE);
				}
			}
			if (flag == SIGNAL_RECEIVED) break;
			fprintf(stdout, "[SERVER] accepted new client %d\n", client_fd);
			fflush(stdout);

			if (i < SOCKETMAXQUEUESIZE)
			{
				err = pthread_create(&(thread[i]), NULL, &capitalize, &client_fd);
				if (err != 0)
				{
					if (flag == SIGNAL_RECEIVED) cleanup_server(server_fd, i);
					else
					{
						perror("pthread_create");
						exit(EXIT_FAILURE);
					}
				}
				i++;
			}
		}
		fprintf(stderr, "\nSIGNAL HAS BEEN RECEIVED. NOW TERMINATING.\n");
		cleanup_server(server_fd, i);
	}
	return 0;
}

void cleanup_server(int server_fd, size_t initialized)
{
	int err;
	EXIT_IF_EQ(err, -1, close(server_fd), close);
	EXIT_IF_EQ(err, -1, remove(SOCKETNAME), remove);
	for (size_t i = 0; i <= initialized; i++)
		EXIT_IF_EQ(err, -1, pthread_cancel(thread[i]), pthread_cancel);
	for (size_t i = 0; i <= initialized; i++)
		EXIT_IF_EQ(err, -1, pthread_join(thread[i], NULL), pthread_join);
	exit(EXIT_SUCCESS);
}

static void* capitalize(void* arg)
{
	int client_fd = *( (int*) arg );
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

	EXIT_IF_EQ(err, -1, close(client_fd), "close");
	return (void*) 0;

}

static void handler(int signum)
{
	static int err;
	if (signum == SIGINT || signum == SIGHUP || signum == SIGTERM || signum == SIGQUIT)
	{
		SIGNALSAFE_EXIT_IF_EQ(err, -1, sigaction(signum, &s, NULL), sigaction);
		flag = SIGNAL_RECEIVED;
	}
}