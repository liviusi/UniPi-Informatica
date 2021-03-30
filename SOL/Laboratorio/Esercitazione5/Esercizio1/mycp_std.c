#include <utilities.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>

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

	char* src_name = argv[1];
	char* dest_name = argv[2];
	FILE* src_f;
	FILE* dest_f;
	FOPEN(src_f, src_name, "r");
	mode_t old_mask = umask(033);
	FOPEN(dest_f, dest_name, "w+");
	umask(old_mask);

	// reading and copying
	char *buffer = (char*) malloc(buffersize*sizeof(char));
	if (!buffer) 
	{
		perror("malloc");
		return errno;
	} 
	size_t len = 0;
	while( (len = fread(buffer, 1, buffersize, src_f)) > 0 )
	{
		if ( (fwrite(buffer, 1, len, dest_f)) != len)
		{
			int tmp = errno;
			perror("fwrite");
			print_errors("writing output file %s : errno = %d\n", dest_name, errno);
			return tmp;
		}
	}

	// freeing allocated resources
	fclose(src_f);
	fflush(dest_f);
	fclose(dest_f);
	free(buffer);

	return 0;

	error_msg:
		fprintf(stderr, "Usage: %s src dest [buffersize]", argv[0]);
		return 1;
}