#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

#define MAX_STRING 64 // stringa di lunghezza massima
#define MAX_PARAMS 256 // massimo numero di parametri
#define CHAR_MALLOC(ptr, size) \
	if ((ptr = (char*) malloc(sizeof(char) * size)) == NULL) \
	{ \
		perror("malloc"); \
		exit(EXIT_FAILURE); \
	}
#define print_dummyshell() \
	do \
	{ \
		fprintf(stdout, "dummyshell >> "); \
		fflush(stdout); \
	} while (0);

char read_command_line(char*, char**);
void free_argv(char**);
char is_exit_command(char*);

int main()
{
	int pid;
	int tmp;
	int status;
	char input_line[MAX_STRING];
	char** argv;

	while (1)
	{
		print_dummyshell();
		memset(input_line, 0, MAX_STRING);
		argv = (char**) calloc(MAX_PARAMS, sizeof(char*));
		tmp = read_command_line(input_line, argv);
		// fprintf(stdout, "tmp = %d\n", tmp);
		if (is_exit_command(input_line))
		{
			fprintf(stdout, "Closing shell...\n");
			free_argv(argv);
			exit(EXIT_SUCCESS);
		}
		switch (tmp)
		{
			case 0:
				pid = fork();
				switch (pid)
				{
					case -1: // errore
						perror("fork");
						exit(EXIT_FAILURE);
					case 0: // figlio
						execvp(argv[0], &(argv[0]));
						perror("execvp");
						exit(errno);
					default:
						if ((pid = waitpid(-1, &status, 0)) == -1)
						{
							perror("waitpid");
							exit(EXIT_FAILURE);
						}
						else if (pid != 0)
						{
							if (WIFEXITED(status))
							{
								fprintf(stdout, "Process %d terminated with status %d.\n", pid, WEXITSTATUS(status));
								fflush(stdout);
							}
						}
						free_argv(argv);
				}
				break;
			case -1:
				exit(tmp);
			case 1:
				fprintf(stdout, "No command has been entered.\n");
				break;
		}
	}

	return 0;
}

void free_argv(char** argv)
{
	size_t i = 0;
	while (argv[i] != NULL)
	{
		free(argv[i]);
		i++;
	}
	free(argv);
	return;
}

char is_exit_command(char* command)
{
	if (strncmp(command, "exit", 4) == 0) return 1;
	else return 0;
}

char read_command_line(char* input_line, char** argv)
{
	if (fgets(input_line, MAX_STRING,stdin) == NULL)
	{
		perror("fgets");
		return -1;
	}

	char* tmp;
	char* token = strtok_r(input_line, " ", &tmp);
	size_t i = 0;
	size_t len = strlen(token) + 1;
	CHAR_MALLOC(argv[i], len);
	strncpy(argv[i], token, len);
	
	while((token = strtok_r(NULL, " ", &tmp)) != NULL)
	{
		if (token[0] != '\0' && token[0] != '\n')
		{
			i++;
			len = strlen(token) + 1;
			CHAR_MALLOC(argv[i], len);
			token[strcspn(token, "\n")] = '\0'; // rimuovo '\n'
			strncpy(argv[i], token, len);
		}
		else continue;
	}

	if (i == 0) return 1; // non esistono comandi da eseguire
	argv[i + 1] = NULL;
	for (size_t j = 0; j <= i + 1; j++) fprintf(stdout, "argv[%lu] = %s\n", j, argv[j]);
	
	return 0; // successo
}