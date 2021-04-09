#include <unistd.h>
#include <stdio.h>
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

	sleep(time);
	fprintf(stdout, "PID = %d\nPPID = %d\n", getpid(), getppid());
	return 0;

}