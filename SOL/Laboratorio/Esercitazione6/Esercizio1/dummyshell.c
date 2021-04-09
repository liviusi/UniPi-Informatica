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

// N.B. da rivedere, valgrind segna troppe free per le malloc fatte.
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
			perror("Cannot exec");
			exit(EXIT_FAILURE);
		}
		default: // padre
		{
			if (waitpid(pid,&status,0) == -1)
			{
				perror("waitpid");
				exit(EXIT_FAILURE);
			}
		}
	}
}

int cmdexit(int argc, char* argv[])
{
	if (argc == 1 && (strcmp(argv[0],"exit") == 0) )
	{
		for (size_t i = 0; i < argc; i++) free(argv[i]);
		free(argv);
		return 0;
	}
	else return 1;
}

int read_cmd_line(int *argc, char* argv[])
{
	char buffer[MAXARG];
	int bufsize = MAXARG;
	fgets(buffer, bufsize, stdin);
	buffer[strlen(buffer)-1] = '\0';
	if (strlen(buffer) > MAXARG) return -1;
	char* s = strtok(buffer," "); 
	while (s)
	{
		argv[*argc] = s;
		(*argc)++;
		s = strtok(NULL," ");
	}
	return 0;
}