#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// macros used for the matrix
#define CONST_FLOAT 2.0f
#define ELEM(M, dim, i, j) *(M + i * dim + j )
#define MATRIX_INITIALIZE(M, N) \
{ \
	for (size_t i = 0; i < dim; i++) \
		for (size_t j = 0; j < dim; j++) \
			ELEM(M, dim, i, j) = (i+j) / CONST_FLOAT; \
}
#define BINARY_DUMP "mat_dump.dat"
#define TEXTUAL_DUMP "mat_dump.txt"

// handling error in fopen
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

char isNumber(const char*);

int confronta(int (*cmp)(const void*, const void*, size_t), const void*, const void*, size_t);

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "Only a positive integer needs to be provided.\n");
		return 1;
	}

	size_t dim;
	if (isNumber(argv[1]))
	{
		if (atoi(argv[1]) <= 0 || atoi(argv[1]) > 512)
		{
			fprintf(stderr, "Given argument is not a positive and valid integer (N > 0 && N < 512).\n");
			return 1;
		}
		dim = atoi(argv[1]);
	}

	float* matrix = (float*) malloc(sizeof(float) * dim * dim);
	if (matrix == NULL) goto malloc_error;
	MATRIX_INITIALIZE(matrix, dim);
	
	FILE* bin_mat;
	FILE* txt_mat;
	FOPEN_DEBUG(bin_mat, BINARY_DUMP, "wb");
	FOPEN_DEBUG(txt_mat, TEXTUAL_DUMP, "w");
	fwrite(matrix, sizeof(float), dim * dim, bin_mat);
	for (size_t i = 0; i < dim * dim; i++)
		fprintf(txt_mat, "%f\n", matrix[i]);

	fclose(bin_mat);
	fclose(txt_mat);

	bin_mat = NULL;
	txt_mat = NULL;

	FOPEN_DEBUG(bin_mat, BINARY_DUMP, "rb");
	FOPEN_DEBUG(txt_mat, TEXTUAL_DUMP, "r");
	float* M1 = (float*) malloc(sizeof(float) * dim * dim);
	if (M1 == NULL) goto malloc_error;
	fread(M1, sizeof(float), dim * dim, bin_mat);
	float* M2 = (float*) malloc(sizeof(float) * dim * dim);
	if (M2 == NULL) goto malloc_error;
	for (size_t i = 0; i < dim * dim; i++)
		fscanf(txt_mat, "%f", &M2[i]);
	
	fprintf(stdout, "Comparing matrices:\n%d.\n", confronta(memcmp, M1, M2, dim * dim * sizeof(float)));
	
	fclose(bin_mat);
	fclose(txt_mat);
	free(matrix);
	free(M1);
	free(M2);

	return 0;

	malloc_error:
		fprintf(stderr, "Malloc failed.\n");
		return 1;
}

char isNumber(const char* str)
{
	for (size_t i = 0; i != strlen(str); i++)
		if (str[i] < '0' || str[i] > '9')
			return 0;
	return 1;
}

int confronta(int (*cmp)(const void*, const void*, size_t), const void* ptr1, const void* ptr2, size_t sz)
{
	return cmp(ptr1, ptr2, sz);
}