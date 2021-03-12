#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SOURCE "/etc/passwd"
#define MAX_LEN 100
#define FOPEN_DEBUG(fname, path, mode) \
{ \
	do \
	{ \
		if ((fname = fopen(path, mode)) == NULL) \
		{ \
			fprintf(stderr, "File %s could not be opened in given mode <%s>.\n", path, mode); \
			return 1; \
		} \
	} while(0); \
}


int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		printf("One more argument is needed. Provide the name for the output file.\n");
		return 1;
	}
	char* name = argv[1]; // name of the output file
	FILE* src;
	FILE* dst;
	FOPEN_DEBUG(src, SOURCE, "r");
	FOPEN_DEBUG(dst, name, "w");

	char* tmp = (char*) malloc(sizeof(char) * MAX_LEN);
	if (tmp == NULL)
	{
		fprintf(stderr, "Malloc failed.\n");
		return 1;
	}
	while (fgets(tmp, MAX_LEN, src) != NULL)
	{
		char* dummy = strchr(tmp, ':');
		*dummy = '\0';
		fprintf(dst, "%s\n", tmp);
	}

	fclose(src);
	fclose(dst);
	free(tmp);

	return 0;
}