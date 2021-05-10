#include <server_api.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <file_data.h>

#define BUCKETS 512
#define UNIX_PATH_MAX 108

static int client_fd;

int openConnection(const char* sockname, int msec, const struct timespec abstime)
{
	struct sockaddr_un socket_address;
	strncpy(socket_address.sun_path, sockname, UNIX_PATH_MAX);

	client_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (client_fd == -1) return -1; // errno has already been set

	time_t start_time, end_time;
	time(&start_time);

	int err;
	while ((err = connect(client_fd, (struct sockaddr*) &socket_address, sizeof(socket_address))) == -1)
	{
		time(&end_time);
		if (difftime(end_time, start_time) > abstime.tv_nsec)
		{
			errno = err;
			return -1;
		}
		sleep(msec);
	}

	return 0;
}

int closeConnection(const char* sockname)
{
	return close(client_fd);
}

int openFile(const char* pathname, int flags)
{
	mode_t oldmask = umask(033);
	int open_flags = 0;
	bool lock = false;
	if (flags & O_CREATE) open_flags |= O_CREAT;
	if (flags & O_LOCK) lock = true;
	int fd = open(pathname, open_flags);
	if (fd == -1) return -1;
	umask(oldmask);
	struct filedata* file = FILEDATA_INIT(fd, pathname, lock);
	return 0;
}

int readFile(const char* pathname, void** buf, size_t* size)
{
	return 0;
}

int writeFile(const char* pathname, const char* dirname)
{
	return 0;
}

int appendToFile(const char* pathname, void* buf, size_t size, const char* dirname)
{
	return 0;
}

int lockFile(const char* pathname)
{
	return 0;
}

int unlockFile(const char* pathname)
{
	return 0;
}

int closeFile(const char* pathname)
{
	return 0;
}

int removeFile(const char* pathname)
{
	return 0;
}