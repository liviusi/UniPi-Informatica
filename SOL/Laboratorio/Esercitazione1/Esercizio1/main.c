#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

void strtoupper(const char* in, size_t len, char* out);

int main(int argc, char* argv[])
{
	if (argc != 2) return 1;
	char* res = malloc(sizeof(char) * (strlen(argv[1])+1));
	strtoupper(argv[1], strlen(argv[1]), res);
	printf("%s\n", res);
	free(res);
	return 0;
}

void strtoupper(const char* in, size_t len, char* out)
{
	out = strncpy(out, in, len);
	for (size_t i = 0; i < len; i++)
	{
		out[i] = toupper( (unsigned char) in[i]);
	}
	out[len] = '\0';
	return;
}