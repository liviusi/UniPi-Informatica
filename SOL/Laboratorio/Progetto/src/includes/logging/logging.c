#include "logging.h"

FILE* init_logging()
{
	FILE* log_file;
	if ((log_file = fopen(LOGGING_FILENAME, "rw+")) == NULL) \
	{
		fprintf(stderr, "Logging could not be initialized.\n");
	}
	return log_file;
}

char write_log(FILE* log_file, const char* buffer, size_t dim)
{
	size_t err;
	if ((err = fwrite(buffer, sizeof(char), dim, log_file)) < dim)
	{
		fprintf(stderr, "Error writing to log.\n");
		return -1;
	}
	return 0;
}

int close_logging(FILE* log_file)
{
	int err;
	if ((err = fclose(log_file)) != 0)
		perror("[close_logging] fclose");
	return err;
}