#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tokenizer.h>

int main(int argc, char* argv[])
{
	if (argc != 2)
		return 1;
	
	size_t len = strlen(argv[1]) + 1;
	char* dummy = (char*) malloc(sizeof(char) * len);
	strncpy(dummy, argv[1], len);

	printf("running tokenizer against %s:\n", dummy);
	tokenizer(dummy);

	strncpy(dummy, argv[1], len);
	printf("running tokenizer_r against %s:\n", dummy);
	tokenizer_r(dummy);
	return 0;
}