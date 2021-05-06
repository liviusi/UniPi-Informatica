#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <utilities.h>

#define BUFFERSIZE 64
#define UNIX_PATH_MAX 108
#define SOCKETMAXQUEUESIZE 5
#define SOCKETNAME "Esercizio1/socket"

int cmd(const char*, char*);

int main(void)
{
	int err;
	int server_fd, client_fd;
	char message[BUFFERSIZE];
	struct sockaddr_un socket_address;

	strncpy(socket_address.sun_path, SOCKETNAME, UNIX_PATH_MAX);
	socket_address.sun_family = AF_UNIX;
	EXIT_IF_EQ(server_fd, -1, socket(AF_UNIX, SOCK_STREAM, 0), "socket");

	bind(server_fd, (struct sockaddr*) &socket_address, sizeof(socket_address));

	char* bc_output;
	EXIT_IF_NULL(bc_output, (char*) malloc(sizeof(char) * BUFFERSIZE), "malloc");
	EXIT_IF_EQ(err, -1, listen(server_fd, SOCKETMAXQUEUESIZE), "listen");
	EXIT_IF_EQ(client_fd, -1, accept(server_fd, NULL, 0), "accept");
	while(true)
	{
		int n;
		
		EXIT_IF_EQ(n, -1, read(client_fd, message, BUFFERSIZE), "read");
		fprintf(stdout, "[SERVER] Expression to evaluate = %s", message);

		memset(bc_output, 0, BUFFERSIZE);
		if ((n = cmd(message, bc_output)) < 0)
		{
			fprintf(stderr, "Error running bc on input %s\n", message);
			exit(EXIT_FAILURE);
		}
		bc_output[n] = '\0';
		fprintf(stdout, "[SERVER] Result = %s", bc_output);

		EXIT_IF_EQ(n, -1, write(client_fd, bc_output, BUFFERSIZE), "write");
	}
	free(bc_output);
	close(server_fd);
	return 0;
}

int cmd(const char* str, char* buffer)
{

	int tobc[2];
	int frombc[2];

	int err;
	RETURN_IF_EQ(err, -1, pipe(tobc), "pipe");
	RETURN_IF_EQ(err, -1, pipe(frombc), "pipe");

	if (fork() == 0)
	{
		// chiudo i descrittori che non uso
		RETURN_IF_EQ(err, -1, close(tobc[1]), "close");  
		RETURN_IF_EQ(err, -1, close(frombc[0]), "close");

		RETURN_IF_EQ(err, -1, dup2(tobc[0], STDIN_FILENO),   "dup2");
		// redirigo sia stdout che stderr sulla stessa pipe
		RETURN_IF_EQ(err, -1, dup2(frombc[1], STDOUT_FILENO), "dup2");
		RETURN_IF_EQ(err, -1, dup2(frombc[1], STDERR_FILENO), "dup2");

		execl("/usr/bin/bc", "bc", "-lq", NULL);
		perror("execl");
		return -1;
	}
	// chiudo i descrittori che non uso
	RETURN_IF_EQ(err, -1, close(tobc[0]), "close");
	RETURN_IF_EQ(err, -1, close(frombc[1]), "close");
	int n;
	RETURN_IF_EQ(n, -1, write(tobc[1], (char*)str,strlen(str)), "write");
	RETURN_IF_EQ(n, -1, read(frombc[0], buffer, BUFFERSIZE), "read");  // leggo il risultato o l'errore
	RETURN_IF_EQ(err, -1, close(tobc[1]), "close");  // si chiude lo standard input di bc cosi' da farlo terminare
	RETURN_IF_EQ(err, -1, wait(NULL), "wait");
	return n;
}