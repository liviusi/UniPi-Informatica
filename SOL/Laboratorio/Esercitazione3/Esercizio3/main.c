#include <stdlib.h>
#include <stdio.h>

#define dimN 16
#define dimM  8

#define CHECK_PTR_EXIT(ptr, str) \
	if (ptr == NULL) { perror(str); exit(EXIT_FAILURE); }

#define ELEM(M, i, j) *(M + i * dimM + j )

#define PRINTMAT(M, dimN, dimM) \
	printf("Stampo la matrice M:\n"); \
	for (size_t i = 0; i < dimN; i++) \
	{ \
		for (size_t j = 0; j < dimM; j++) \
			printf("%ld  ", ELEM(M, i, j)); \
		printf("\n"); \
	}

int main(int argc, char* argv[])
{
	long *M = malloc(dimN*dimM*sizeof(long));
	CHECK_PTR_EXIT(M, "malloc");
	for(size_t i=0;i<dimN;++i)
		for(size_t j=0;j<dimM;++j)
			ELEM(M,i,j) = i+j;

	PRINTMAT(M, dimN, dimM);
	free(M);
	return 0;
}