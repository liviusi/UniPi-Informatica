#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <utilities.h>

#define BUFFERSIZE 64
#define UNIX_PATH_MAX 108
#define SOCKETMAXQUEUESIZE 5
#define SOCKETNAME "Esercizio2/socket"
#define EXITCOMMAND "exit"

int main(void)
{
	int client_fd;
	char buffer[BUFFERSIZE];
	struct sockaddr_un socket_address;

	strncpy(socket_address.sun_path, SOCKETNAME, UNIX_PATH_MAX);
	socket_address.sun_family = AF_UNIX;
	EXIT_IF_EQ(client_fd, -1, socket(AF_UNIX, SOCK_STREAM, 0), "socket");

	while(connect(client_fd, (struct sockaddr*) &socket_address, sizeof(socket_address)) == -1)
	{
		if (errno == ENOENT)
		{
			fprintf(stderr, "Cannot connect to server as it has yet to be created.\nTrying again in 5s.\n");
			sleep(5);
		}
		else exit(EXIT_FAILURE);
	}

	while(true)
	{
		ssize_t n;
		memset(buffer, 0, BUFFERSIZE);
		if (fgets(buffer, BUFFERSIZE, stdin) != NULL)
		{
			fprintf(stdout, "[CLIENT] Writing to server: %s", buffer);
			EXIT_IF_EQ(n, -1, write(client_fd, buffer, BUFFERSIZE), "write"); // write expression to server

			if (strncmp(buffer, EXITCOMMAND, strlen(EXITCOMMAND)) == 0) break;

			EXIT_IF_EQ(n, -1, read(client_fd, buffer, BUFFERSIZE), "read"); // read result from server
			fprintf(stdout, "[CLIENT] Reading from server: %s", buffer);
		}
	}

	close(client_fd);
	return 0;
}