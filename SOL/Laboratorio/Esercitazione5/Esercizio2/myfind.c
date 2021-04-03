#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <utilities.h>
#include <dirent.h>
#include <time.h>

#define MAX_LENGTH 256

char dots_only(const char*);
char find(const char*, const char*);
char* _cwd();

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		fprintf(stderr, "Usage: %s dir filename\n", argv[0]);
		return 1;
	}

	char* dir_name = argv[1];
	char* filename = argv[2];

	int r;
	struct stat statbuf;
	SYSCALL(stat, r, stat(dir_name, &statbuf), "calling stat on %s, errno = %d\n", dir_name, errno);

	if (!S_ISDIR(statbuf.st_mode))
	{
		fprintf(stdout, "%s is not a directory\n", dir_name);
		return 1;
	}

	find(dir_name, filename);

	return 0;
}

/**
 * @brief checks whether str is only made up of dots.
 * @returns 1 if str is ".", ".." etc, 0 otherwise
**/
char dots_only(const char* str)
{
	for (size_t i = 0; str[i] != '\0'; i++)
		if (str[i] != '.') return 0;
	return 1;
}

char* _cwd()
{
	char* buffer = (char*) malloc(sizeof(char) * MAX_LENGTH);
	if (buffer == NULL)
	{
		perror("malloc");
		fprintf(stderr, "malloc failed.\n");
		return NULL;
	}
	getcwd(buffer, MAX_LENGTH);
	return buffer;
}

char find(const char* dir_name, const char* filename)
{
	// enter dir_name
	if (chdir(dir_name) == -1)
	{
		fprintf(stderr, "ERROR: cannot enter directory %s\n", dir_name);
		return -1;
	}

	// open dir_name
	DIR* dir;
	if ( (dir = opendir(".")) == NULL )
	{
		fprintf(stderr, "ERROR: cannot open directory %s\n", dir_name);
		return -1;
	}

	// listing files
	struct dirent* file;
	int r;
	struct stat statbuf;
	while ( (errno = 0, file = readdir(dir)) != NULL )
	{
		SYSCALL(stat, r, stat(file->d_name, &statbuf), "calling stat on %s, errno = %d\n", file->d_name, errno);
		if (!dots_only(file->d_name))
		{
			if (S_ISDIR(statbuf.st_mode) && find(file->d_name, filename) != 0)
			{
				if (chdir("..") == -1)
					return -1;
			}
			if (S_ISREG(statbuf.st_mode))
			{
				char* buffer = _cwd();
				if (!buffer)
					return -1;
				if (strncmp(file->d_name, filename, strlen(file->d_name)) == 0)
					fprintf(stdout, "%s/%s %s\n", buffer, file->d_name, ctime(&statbuf.st_mtime));
			}
		}
	}
	if (errno != 0)
	{
		perror("readdir");
		return -1;
	}
	closedir(dir);
	return 1;
}