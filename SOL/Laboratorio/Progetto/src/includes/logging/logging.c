#include "logging.h"
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

FILE* init_logging(const char* log_filename)
{
	FILE* log_file;
	if ((log_file = fopen(log_filename, "w+")) == NULL) \
	{
		fprintf(stderr, "Logging could not be initialized.\n");
	}
	return log_file;
}

char write_log(FILE* log_file, const char* buffer)
{
	size_t err;
	mode_t old_mask = umask(033);
	size_t len = strlen(buffer);
	if ((err = fwrite(buffer, sizeof(char), len, log_file)) < len)
	{
		fprintf(stderr, "Error writing to log.\n");
		return -1;
	}
	umask(old_mask);
	return 0;
}

int close_logging(FILE* log_file)
{
	int err;
	if ((err = fclose(log_file)) != 0)
		perror("[close_logging] fclose");
	return err;
}

/* int main(void)
{
	const char name[] = "log.txt";
	FILE* log;
	if ((log = init_logging(name)) == NULL)
	{
		fprintf(stderr, "[init_logging] * visible sweating *\n");
		return 1;
	}
	char buffer[] = "message1\n";
	if (write_log(log, buffer) != 0)
	{
		fprintf(stderr, "[write_log] * visible sweating *\n");
		return 1;
	}
	if (close_logging(log) != 0)
	{
		fprintf(stderr, "[close_logging] * visible sweating *\n");
		return 1;
	}
	return 0;
} */