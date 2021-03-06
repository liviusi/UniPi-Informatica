#include <stdio.h>
#include <stdlib.h>

int somma(int x);

int somma_r(int x, int *res);

int main(int argc, char* argv[])
{
	if (argc != 2 || atoi(argv[1]) <= 0)
	{
		printf("Must provide a positive integer.");
		return 1;
	}
	int x;
	#ifdef INIT_VALUE
	int save = INIT_VALUE;
	#else
	int save = 0;
	#endif
	size_t len = atoi(argv[1]);
	for (size_t i = 0; i < len; i++)
	{
		scanf("%d", &x);
		printf("Result: %d\n", somma(x));
		printf("Reentrant version: %d\n", somma_r(x, &save));
	}
	return 0;
}

int somma(int x)
{
	#ifdef INIT_VALUE
	static int res = INIT_VALUE;
	#else
	static int res = 0;
	#endif

	res += x;
	return res;
}

int somma_r(int x, int* res)
{
	int tmp = *res + x;
	*res = tmp;
	return *res;
}