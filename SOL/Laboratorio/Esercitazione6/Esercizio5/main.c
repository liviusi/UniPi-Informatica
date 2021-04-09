#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/* 
 * Calcola ricorsivamente il numero di Fibonacci dell'argomento 'n'.
 * La soluzione deve effettuare fork di processi con il vincolo che 
 * ogni processo esegua 'doFib' al più una volta.  
 * Se l'argomento doPrint e' 1 allora la funzione stampa il numero calcolato 
 * prima di passarlo al processo padre. 
*/
static void doFib(int n, int doPrint);
int get_exit_code(int);

int main(int argc, char *argv[])
{
	// questo programma puo' calcolare i numeri di Fibonacci solo fino a 13.  
	const int NMAX=13;
	int arg;

	if(argc != 2)
	{
		fprintf(stderr, "Usage: %s <num>\n", argv[0]);
		return EXIT_FAILURE;
	}
	arg = atoi(argv[1]);
	if(arg <= 0 || arg > NMAX)
	{
		fprintf(stderr, "num deve essere compreso tra 1 e 13\n");
		return EXIT_FAILURE;
	}
	doFib(arg, 1);
	return 0;
}

int get_exit_code(int pid)
{
	int status;
	if (waitpid(pid, &status, 0) == -1)
	{
		perror("wait");
		_exit(EXIT_FAILURE);
	}
	
	if (WIFEXITED(status)) return WEXITSTATUS(status);

	_exit(EXIT_FAILURE);
}

static void doFib(int n, int doPrint)
{
	if (n < 3) // il primo termine è Fib(1) e non Fib(0)
	{
		if (doPrint == 1) fprintf(stdout, "1\n");
		_exit(1);
	}
	else
	{
		int pid1, pid2;
		
		if ((pid1 = fork()) == -1)
		{
			perror("fork");
			_exit(EXIT_FAILURE);
		}
		if ((pid2 = fork()) == -1)
		{
			perror("fork");
			_exit(EXIT_FAILURE);
		}

		if (pid1 == 0) doFib(n-1, 0);
		if (pid2 == 0) doFib(n-2, 0);

		int res1 = get_exit_code(pid1);
		int res2 = get_exit_code(pid2);
		if (doPrint == 1) fprintf(stdout, "%d\n", res1 + res2);
		_exit(res1 + res2);
	
	}
}