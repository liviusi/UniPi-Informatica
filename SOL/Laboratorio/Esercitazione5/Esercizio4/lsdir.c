#define _POSIX_C_SOURCE  200112L  // needed for S_ISSOCK
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <utilities.h>

#define MAX_BUFFER_LENGTH 512

struct char_list
{
	char* name;
	struct char_list* next;
};

void add_head(char*, struct char_list**);
void free_list(struct char_list**);
char dots_only(const char*);
char print_directory(const char*);
void get_permissions(const struct stat, char[]);
void print_regular_file_info(const struct stat, const struct dirent*);

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s dir", argv[0]);
		return 1;
	}

	int r;
	struct stat statbuf;
	SYSCALL(stat, r, stat(argv[1], &statbuf), "calling stat on %s, errno = %d\n", argv[1], errno);
	if (!S_ISDIR(statbuf.st_mode))
	{
		fprintf(stderr, "ERROR: %s is not a directory.", argv[1]);
		return 1;
	}

	return print_directory(argv[1]);
}

void add_head(char* str, struct char_list** lst)
{
	if (!(*lst))
	{
		*lst = (struct char_list*) malloc(sizeof(struct char_list));
		(*lst)->name = str;
		(*lst)->next = NULL;
		return;
	}
	struct char_list* new_head = (struct char_list*) malloc(sizeof(struct char_list));
	new_head->name = str;
	new_head->next = *lst;
	*lst = new_head;
}

void free_list(struct char_list** lst)
{
	if (*lst)
	{
		struct char_list* tmp = *lst;
		*lst = (*lst)->next;
		free(tmp);
		free_list(lst);
	}
}

char dots_only(const char* str)
{
	for (size_t i = 0; str[i] != '\0'; i++)
		if (str[i] != '.') return 0;
	return 1;
}

void get_permissions(const struct stat statbuf, char mode[])
{
	if (S_ISDIR(statbuf.st_mode)) mode[0]='d';
	if (S_ISCHR(statbuf.st_mode)) mode[0]='c';
	if (S_ISBLK(statbuf.st_mode)) mode[0]='b';
	if (S_ISLNK(statbuf.st_mode)) mode[0]='l';
	if (S_ISFIFO(statbuf.st_mode)) mode[0]='p';
	if (S_ISSOCK(statbuf.st_mode)) mode[0]='s';
	if (S_IRUSR & statbuf.st_mode) mode[1]='r';
	if (S_IWUSR & statbuf.st_mode) mode[2]='w';
	if (S_IXUSR & statbuf.st_mode) mode[3]='x';
	if (S_IRGRP & statbuf.st_mode) mode[4]='r';
	if (S_IWGRP & statbuf.st_mode) mode[5]='w';
	if (S_IXGRP & statbuf.st_mode) mode[6]='x';
	if (S_IROTH & statbuf.st_mode) mode[7]='r';
	if (S_IWOTH & statbuf.st_mode) mode[8]='w';
	if (S_IXOTH & statbuf.st_mode) mode[9]='x';
}

void print_regular_file_info(const struct stat statbuf, const struct dirent* file)
{
	char mode[11] = {'-','-','-','-','-','-','-','-','-','-','\0'};
	get_permissions(statbuf, mode);
	fprintf(stdout, "%s %ld %s\n", file->d_name, statbuf.st_size, mode);
}

char print_directory(const char* root)
{
	fprintf(stdout, "Directory: %s\n", root);
	
	// enter dir_name
	if (chdir(root) == -1)
	{
		fprintf(stderr, "ERROR: cannot enter directory %s\n", root);
		return -1;
	}

	// open directory
	DIR* dir;
	if ( (dir = opendir(".")) == NULL )
	{
		fprintf(stderr, "ERROR: cannot open directory %s\n", root);
		return -1;
	}

	// loop through regular files
	struct dirent* file;
	int r;
	struct stat statbuf;
	while ( (errno = 0, file = readdir(dir)) != NULL )
	{
		if (!dots_only(file->d_name))
		{
			SYSCALL(stat, r, stat(file->d_name, &statbuf), "calling stat on %s, errno = %d\n", file->d_name, errno);
			if (S_ISREG(statbuf.st_mode))
				print_regular_file_info(statbuf, file);
		}
	}
	if (errno != 0)
	{
		perror("readdir");
		return -1;
	}

	fprintf(stdout, "------------------\n");
	// dirp is now useless
	closedir(dir);

	// reopen directory
	if ( (dir = opendir(".")) == NULL )
	{
		fprintf(stderr, "ERROR: cannot open directory %s\n", root);
		return -1;
	}

	// loop through subdirectories
	struct char_list* lst = NULL;
	while ( (errno = 0, file = readdir(dir)) != NULL )
	{
		if (!dots_only(file->d_name))
		{
			// char dummy[MAX_BUFFER_LENGTH];
			// getcwd(dummy, MAX_BUFFER_LENGTH);
			// fprintf(stdout, "cwd : %s\nstat will be run on %s / %s\n", dummy, root, file->d_name);
			SYSCALL(stat, r, stat(file->d_name, &statbuf), "calling stat on %s, errno = %d\n", file->d_name, errno);
			if (S_ISDIR(statbuf.st_mode))
			{
				if (MAX_BUFFER_LENGTH > strlen(file->d_name) + strlen(root) + 1)
				{
					char* tmp = (char*) malloc(sizeof(char) * MAX_BUFFER_LENGTH);
					size_t len = strlen(root);
					if (root[strlen(root) - 1] == '/') len--;
					strncpy(tmp, root, len);
					tmp[len] = '/';
					strncpy(tmp + len + 1, file->d_name, strlen(file->d_name));
					tmp[len + 1 + strlen(file->d_name)] = '/';
					tmp[len + 1 + strlen(file->d_name) + 1] = '\0';
					add_head(tmp, &lst);
				}
				else
				{
					fprintf(stderr, "Buffer size is too small for path to %s", file->d_name);
					return 1;
				}
			}
		}
	}
	if (errno != 0)
	{
		perror("readdir");
		return -1;
	}
	// fprintf(stdout, "%s will now be closed.\n", root);
	closedir(dir);

	for (struct char_list* i = lst; i != NULL; i = i->next)
		print_directory(i->name);

	free_list(&lst);

	return 0;
}