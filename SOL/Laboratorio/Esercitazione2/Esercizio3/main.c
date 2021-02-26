#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
	if (argc != 3)
	{
		fprintf(stderr, "use: %s stringa1 stringa2\n", argv[0]);
		return -1;
	}
	char* saveptr1 = NULL;
	char* saveptr2 = NULL;
	char* _str2 = (char*) calloc(strlen(argv[2]) + 1, sizeof(char));
	char* token1 = strtok_r(argv[1], " ", &saveptr1);
	while (token1)
	{
		printf("%s\n", token1);
		strncpy(_str2, argv[2], strlen(argv[2]));
		_str2[strlen(argv[2])] = '\0';
		char* token2 = strtok_r(_str2, " ", &saveptr2);
		while(token2)
		{
			printf("%s\n", token2);
			token2 = strtok_r(NULL, " ", &saveptr2);
		}
		token1 = strtok_r(NULL, " ", &saveptr1);
	}
	free(_str2);
	return 0;
}