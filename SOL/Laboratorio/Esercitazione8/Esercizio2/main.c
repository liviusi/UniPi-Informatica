#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <utilities.h>

#define OPERAZIONE "Operazione: "
#define RISULTATO "Risultato: "
#define MAXSTRING 128

void close_pipefd(int*);
void run_calculator(char*);

int main(void)
{
	char* buffer;
	const char exit_cmd[] = "exit";
	EXIT_IF_NULL(buffer, (char*) malloc(MAXSTRING * sizeof(char)), "malloc");
	while (true)
	{
		memset(buffer, 0, MAXSTRING);
		fprintf(stdout, OPERAZIONE);
		EXIT_IF_NULL(buffer, fgets(buffer, MAXSTRING, stdin), "fgets");
		if (strncmp(buffer, exit_cmd, strlen(exit_cmd)) == 0) break;
		//fprintf(stdout, RISULTATO);
		run_calculator(buffer);
	}

	free(buffer);
	return 0;
}

void close_pipefd(int pipefd[2])
{
	close(pipefd[0]);
	close(pipefd[1]);
}

void run_calculator(char* buffer)
{
	int pipefd[2], err;
	pid_t pid_echo, pid_bc, _aux;

	EXIT_IF_EXPECTED_VALUE(err, -1, pipe(pipefd), "pipe");

	switch (pid_echo = fork())
	{
		case -1:
			perror("fork"); 
			exit(EXIT_FAILURE);

		case 0:
			close(STDIN_FILENO);
			EXIT_IF_EXPECTED_VALUE(err, -1, dup2(pipefd[1], STDOUT_FILENO), "dup2");
			// stdout = pipefd[1]
			// can now close pipefd
			close(pipefd[0]);
			close(pipefd[1]);
			printf("%s", buffer);
			exit(EXIT_SUCCESS);

		default:
			break;
	}

	switch (pid_bc = fork())
	{
			case -1:
				perror("fork");
				exit(EXIT_FAILURE);

			case 0:
				//EXIT_IF_EXPECTED_VALUE(err, -1, dup2(pipefd[1], STDERR_FILENO), "dup2");
				//EXIT_IF_EXPECTED_VALUE(err, -1, dup2(pipefd[1], STDOUT_FILENO), "dup2");
				EXIT_IF_EXPECTED_VALUE(err, -1, dup2(pipefd[0], STDIN_FILENO), "dup2");
				// stdin = pipefd[0]
				// stderr, stdout = pipefd[1]
				close(pipefd[0]);
				close(pipefd[1]);
				execlp("bc", "bc", "-lq", (char*) NULL);
				exit(errno);

			default:
				break;
	}

	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid_echo, NULL, 0);
	waitpid(pid_bc, NULL, 0);

	return;
}