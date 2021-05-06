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

#include <wrappers.h>

#define SOCKETNAME "FileStorageSocket"
#define UNIX_PATH_MAX 108
#define SOCKETMAXQUEUESIZE 5

int main(void)
{
	int err;
	int server_fd, client_fd;
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
	}

	EXIT_IF_EQ(err, -1, close(server_fd), "close");
	return 0;
}
