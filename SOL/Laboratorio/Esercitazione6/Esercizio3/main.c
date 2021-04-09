#include <stdio.h>
#include <unistd.h>
#include <utilities.h>

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
		fprintf(stderr, "ERROR: N is not a positive integer");
		return 1;
	}

	int pid = 1;

	for (size_t i = 0; i < N; i++)
	{
		if (pid != 0) pid = fork();
	}

	if (pid > 0) sleep(5);

	return 0;
}