#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <utilities.h>

#define BUFFER_DEFAULT_SIZE 256

int main(int argc, char* argv[])
{
	// handling input
	if (argc > 4 || argc < 2)
		goto error_msg;
	long buffersize = BUFFER_DEFAULT_SIZE;
	if (argc == 4)
	{
		if (isNumber(argv[argc - 1], &buffersize) != 0)
			goto error_msg;
	}

	// opening and creating needed files
	int src_fd, dest_fd;
	char* src_name = argv[1];
	char* dest_name = argv[2];
	mode_t old_mask = umask(033);
	SYSCALL("open", src_fd, open(src_name, O_RDONLY), "opening input file %s : errno = %d\n", src_name, errno);
	SYSCALL("open", dest_fd, open(dest_name, O_CREAT|O_WRONLY|O_EXCL, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH), "creating output file %s : errno = %d\n", dest_name, errno);
	umask(old_mask);

	// reading and copying
	char *buffer = (char*) malloc(buffersize*sizeof(char));
	if (!buffer) 
	{
		perror("malloc");
		return errno;
	} 
	size_t len;
	do
	{
		SYSCALL(read, len, read(src_fd, buffer, buffersize), "reading input file %s : errno = %d\n", src_name, errno);
		if (len)
			SYSCALL(write, len, write(dest_fd, buffer, len), "writing output file %s : errno = %d\n", dest_name, errno);
	} while(len>0);

	// freeing allocated resources
	SYSCALL(close, len, close(src_fd), "closing input file %s : errno = %d\n", src_name, errno);
	SYSCALL(close, len, close(dest_fd), "closing output file %s : errno = %d\n", dest_name, errno);
	free(buffer);

	return 0;

	error_msg:
		fprintf(stderr, "Usage: %s src dest [buffersize]\n", argv[0]);
		return 1;
}