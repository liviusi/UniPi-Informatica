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
#include <stdlib.h>
#include <errno.h>

#define EXIT_IF_NEQ(variable, expected_value, function_call, error_message) \
	do \
	{ \
		if ((variable = function_call) != expected_value) \
		{ \
			errno = variable; \
			perror(#error_message); \
			exit(EXIT_FAILURE); \
		} \
	} while(0);

#define EXIT_IF_EQ(variable, expected_value, function_call, error_message) \
	do \
	{ \
		if ((variable = function_call) == expected_value) \
		{ \
			perror(#error_message); \
			exit(EXIT_FAILURE); \
		} \
	} while(0);

#define RETURN_IF_EQ(variable, expected_value, function_call, error_message) \
	if ((variable = function_call) == expected_value) \
	{ \
		perror(#error_message); \
		return variable; \
	}

#define EXIT_IF_NULL(variable, function_call, error_message) \
	do \
	{ \
		if ((variable = function_call) == NULL) \
		{ \
				perror(#error_message); \
				exit(EXIT_FAILURE); \
		} \
	} while(0);

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
