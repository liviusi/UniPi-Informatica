#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <utilities.h>

char isNumber(const char*, unsigned int*);

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s time\n", argv[0]);
		return 1;
	}

	unsigned int time;
	if (isNumber(argv[1], &time) != 0)
	{
		fprintf(stderr, "ERROR: time is not a positive integer.\n");
		return 1;
	}

	int pid = fork();
	if (pid == 0) // figlio
	{
		if ((pid = fork()) == 0) // figlio
		{
			execl("/bin/sleep", "/bin/sleep", argv[1], NULL);
			perror("execl");
			exit(EXIT_FAILURE);
		}
		if (waitpid(pid, NULL, 0) == -1)
		{
			perror("waitpid");
			exit(EXIT_FAILURE);
		}
		fprintf(stdout, "PID = %d\nPPID = %d\n", getpid(), getppid());
	}
	return 0;

}