#include <stdio.h>
#include <ctype.h>
#include <getopt.h>
#include <stdlib.h>

#define WC_PRINT(l, w, filename) \
{ \
	do \
	{ \
		if (no_flags) fprintf(stdout, "%lu %lu ", l, w); \
		else \
		{ \
			if (l_flag) fprintf(stdout, "%lu ", l); \
			if (w_flag) fprintf(stdout, "%lu ", w); \
		} \
		fprintf(stdout, "%s\n", filename); \
	} while(0); \
}

#define FOPEN_DEBUG(fname, path, mode) \
{ \
	do \
	{ \
		if ((fname = fopen(path, mode)) == NULL) \
		{ \
			fprintf(stderr, "File %s could not be opened in given mode <%s>.\n", path, mode); \
			exit(EXIT_FAILURE); \
		} \
	} while(0); \
}

void wc(size_t*, size_t*, const char*);

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		fprintf(stderr, "A positional argument is required. Provide a valid file to read from.\n");
		return -1;
	}
	char l_flag = 0, w_flag = 0, no_flags = 1;
	
	int opt;
	while((opt = getopt(argc, argv, "lw")) != -1)
	{
		switch (opt)
		{
			case 'w':
				no_flags = 0;
				w_flag = 1;
				break;
			case 'l':
				no_flags = 0;
				l_flag = 1;
				break;
			case '?':
				fprintf(stderr, "Option <%c> is not valid.\n", optopt);
				return -1;
		}
	}

	// f_ shorthand for single file
	// t_ shorthand for total
	size_t f_words, f_newlines, t_words, t_newlines;
	f_words = f_newlines = t_words = t_newlines = 0;
	// fprintf(stdout, "Number of files to be parsed: %d.\n", argc - optind);
	for (size_t i = optind; i < argc; i++)
	{
		wc(&f_words, &f_newlines, argv[i]);
		t_words += f_words;
		t_newlines += f_newlines;
		WC_PRINT(f_newlines, f_words, argv[i]);
	}

	if (argc - optind != 0) WC_PRINT(t_newlines, t_words, "total");

	return 0;
}

void wc(size_t* words, size_t* newlines, const char* filename)
{
	*words = 0;
	*newlines = 0;

	FILE* in;
	FOPEN_DEBUG(in, filename, "r");

	char c;
	char isword = 0;
	while ((c = fgetc(in)) != EOF)
	{
		if (!isspace(c) && !isword)
		{
			isword = 1;
			(*words)++;
		}

		if (isspace(c) && isword) isword = 0;

		if (c == '\n') (*newlines)++;
	}

	fclose(in);
}