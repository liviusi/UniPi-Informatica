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
#include <utilities.h>

#define BUFFERSIZE 64
#define UNIX_PATH_MAX 108
#define SOCKETMAXQUEUESIZE 5
#define SOCKETNAME "Esercizio2/socket"
#define EXITCOMMAND "exit"

static void* capitalize(void*);
pthread_t thread[SOCKETMAXQUEUESIZE];

int main(void)
{
	int err;
	int server_fd, client_fd;
	size_t i = 0;
	struct sockaddr_un socket_address;

	strncpy(socket_address.sun_path, SOCKETNAME, UNIX_PATH_MAX);
	socket_address.sun_family = AF_UNIX;
	EXIT_IF_EQ(server_fd, -1, socket(AF_UNIX, SOCK_STREAM, 0), "socket");
	EXIT_IF_EQ(err, -1, bind(server_fd, (struct sockaddr*) &socket_address, sizeof(socket_address)), "bind");

	while(true)
	{
		fprintf(stdout, "[SERVER] listening\n");
		EXIT_IF_EQ(err, -1, listen(server_fd, SOCKETMAXQUEUESIZE), "listen");
		EXIT_IF_EQ(client_fd, -1, accept(server_fd, NULL, 0), "accept");
		fprintf(stdout, "[SERVER] accepted new client %d\n", client_fd);

		if(i < SOCKETMAXQUEUESIZE)
		{
			EXIT_IF_NEQ(err, 0, pthread_create(&(thread[i]), NULL, &capitalize, &client_fd), "pthread_create");
			i++;
		}
		else break;
	}

	EXIT_IF_EQ(err, -1, close(server_fd), "close");
	return 0;
}

static void* capitalize(void* arg)
{
	int client_fd = *( (int*) arg );
	char message[BUFFERSIZE];
	ssize_t n;
	int err;

	EXIT_IF_NEQ(err, 0, pthread_detach(pthread_self()), "pthread_detach");
	while(true)
	{
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