#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define MAXARG 512

static void execute (int, char**);
int cmdexit(int, char**);
int read_cmd_line(int*, char**);


int main(void)
{
	int argc = 0;
	char** argv;
	argv = (char**) malloc(MAXARG * sizeof(char*));

	while (1)
	{
		printf ("dummyshell >>> ");
		if (read_cmd_line(&argc, argv) != -1)
		{
			if (cmdexit(argc, argv) == 0) exit(EXIT_SUCCESS);
			argv[sizeof(argv-1)] = NULL;
			execute(argc, argv);
		}
		else fprintf (stderr, "invalid command!\n");
		for (size_t i = 0; i < argc; i++)
			argv[i] = NULL;
		argc = 0;
	}

	return 1;
}

static void execute(int argc, char* argv [])
{
	pid_t pid;
	int status;
	switch (pid = fork())
	{
		case -1: // errore processo padre
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		case 0: // figlio
		{
			execvp(argv[0], argv);
			perror("exec");
			exit(EXIT_FAILURE);
		}
		default: // padre
		{
			if (waitpid(pid,&status,0) == -1)
			{
				perror("waitpid");
				exit(EXIT_FAILURE);
			}
			for (size_t i = 0; i < argc - 1; i++) free(argv[i]);
		}
	}
}

int cmdexit(int argc, char* argv[])
{
	if (argc == 1 && (strncmp(argv[0], "exit", strlen("exit")) == 0) )
	{
		for (size_t i = 0; i <= argc; i++) free(argv[i]);
		free(argv);
		return 0;
	}
	else return 1;
}

int read_cmd_line(int *argc, char* argv[])
{
	char* buffer = (char*) malloc(sizeof(char) * MAXARG);
	int bufsize = MAXARG;
	fgets(buffer, bufsize, stdin);
	buffer[MAXARG - 1] = '\0';
	char* s = strtok(buffer, " "); 
	while (s)
	{
		s[strcspn(s, "\n")] = '\0'; // rimuovo '\n'
		argv[*argc] = s;
		(*argc)++;
		s = strtok(NULL, " ");
	}
	argv[*argc] = (char*) NULL;
	for (size_t i = 0; i <= *argc; i++) fprintf(stdout, "argv[%lu] = %s\n", i, argv[i]);
	return 0;
}