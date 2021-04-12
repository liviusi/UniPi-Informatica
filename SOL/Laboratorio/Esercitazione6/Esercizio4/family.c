#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <utilities.h>

#define _print_n_(n) \
	do \
	{ \
		for(size_t i = 0; i < n; i++) fprintf(stdout, "-");\
	} while(0);

void print_family(unsigned int);

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s N\n", argv[0]);
		return 1;
	}

	unsigned int N;
	if (isNumber(argv[1], &N) != 0)
	{
		fprintf(stderr, "ERROR: N is not a positive integer.\n");
		return 1;
	}

	print_family(N);
	
	return 0;
}

void print_family(unsigned int N)
{
	int pid;
	
	if (N == 0)
	{
		fprintf(stdout, "%d: sono l'ultimo discendente.\n", getpid());
		fprintf(stdout, "%d: terminato con successo.\n", getpid());
		return;
	}

	if ((pid = fork()) == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}

	if (pid == 0) print_family(N - 1); // processo figlio
	else // processo padre
	{
		_print_n_(N);
		fprintf(stdout, " %d: creo un processo figlio\n", getpid());
		fflush(stdout);
		if (waitpid(pid, (int*)NULL, 0) == -1)
		{
			perror("wait");
			exit(EXIT_FAILURE);
		}
		_print_n_(N);
		fprintf(stdout, " %d: terminato con successo.\n", getpid());
		fflush(stdout);
	}
}