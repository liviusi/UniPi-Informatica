#define _POSIX_C_SOURCE 199309L

#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>

#define SYSCALL(function, x, call) { if ((x = call) == -1) { perror(#function); _exit(1); } }
#define BUFFERLEN 8

volatile sig_atomic_t count_sigint = 0;
volatile sig_atomic_t count_sigtstp = 0;

static void handler(int);

int main(void)
{
	int err;
	struct sigaction s;
	memset(&s, 0, sizeof(s));
	s.sa_handler = handler;
	SYSCALL(sigaction, err, sigaction(SIGINT, &s, NULL));
	SYSCALL(sigaction, err, sigaction(SIGTSTP, &s, NULL));

	while(1) { }
	return 0;
}

static void handler(int signum)
{
	static int err;
	static char buffer[BUFFERLEN];
	static struct timeval timeout;
	static fd_set readfds;
	FD_ZERO(&readfds);
	FD_SET(0, &readfds);
	timeout.tv_sec = 10;
	timeout.tv_usec = 0;
	switch (signum)
	{
		case SIGINT:
			count_sigint++;
			break;

		case SIGTSTP:
			count_sigtstp++;
			if (count_sigtstp == 3)
			{
				for (int i = BUFFERLEN - 3; i >= 0; --i, count_sigint /= 10)
					buffer[i] = (count_sigint % 10) + '0';
				buffer[BUFFERLEN - 1] = '\0';
				buffer[BUFFERLEN - 2] = '\n';
				buffer[0] = '\n';
				SYSCALL(write, err, write(STDIN_FILENO, buffer, strlen(buffer)));
				SYSCALL(select, err, select(STDOUT_FILENO, &readfds, NULL, NULL, &timeout));
				SYSCALL(read, err, read(STDOUT_FILENO, buffer, strlen(buffer)));
				if (err != 0) kill(getpid(), SIGALRM);
			}
			break;

		default:
			break;
	}
}