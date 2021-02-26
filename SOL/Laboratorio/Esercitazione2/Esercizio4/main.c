#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#define N 1000
#define K1 100
#define K2 120
#define UNSIGNED_ERROR "deve essere un numero intero positivo.\n"
#define USAGE_ERROR "Possono essere forniti al più 3 argomenti opzionali interi e positivi.\n"
#define INTERVALLO_ERROR "L'intervallo deve essere della forma [K1; K2[.\n"

char is_digit(char* str); // usata per verificare che i valori in input siano numeri
int compare(const void* a, const void* b); // usata per qsort
int ricerca_binaria(unsigned int arr[], unsigned int dim, unsigned int x, char first); // usata per contare le occorrenze
int conta_occorrenze(unsigned int* arr, unsigned int dim, int x); // conta le occorrenze


int main(int argc, char* argv[])
{
	unsigned int n, k1, k2;
	switch (argc)
	{
		case 1:
			n = N;
			k1 = K1;
			k2 = K2;
			break;
		case 2:
			if (!(is_digit(argv[1])))
			{
				printf("N "UNSIGNED_ERROR);
				return 1;
			}
			n = atoi(argv[1]);
			k1 = K1;
			k2 = K2;
			break;
		case 3:
			if (!(is_digit(argv[1])))
			{
				printf("N "UNSIGNED_ERROR);
				return 1;
			}
			if (!(is_digit(argv[2])))
			{
				printf("K1 "UNSIGNED_ERROR);
				return 1;
			}
			n = atoi(argv[1]);
			k1 = atoi(argv[2]);
			k2 = K2;
			break;
		case 4:
			if (!(is_digit(argv[1])))
			{
				printf("N "UNSIGNED_ERROR);
				return 1;
			}
			if (!(is_digit(argv[2])))
			{
				printf("K1 "UNSIGNED_ERROR);
				return 1;
			}
			if (!(is_digit(argv[3])))
			{
				printf("K2 "UNSIGNED_ERROR);
				return 1;
			}
			n = atoi(argv[1]);
			k1 = atoi(argv[2]);
			k2 = atoi(argv[3]);
			break;
		default:
			printf(USAGE_ERROR);
			return 1;
	}
	if (k1 >= k2)
	{
		printf(INTERVALLO_ERROR);
		return 1;
	}
	unsigned int seed = 42; // seme iniziale non casuale per avere testcase consistenti
	unsigned int* arr = (unsigned int*) malloc(sizeof(unsigned int) * n);
	for (size_t i = 0; i < n; i++) // genera i numeri
		arr[i] = k1 + rand_r(&seed) % (k2 - k1); // n \in [K1; K2[
	qsort(arr, n, sizeof(unsigned int), compare);
	printf("Occorrenze di:\n");
	for (size_t i = k1; i < k2; i++)
		printf("%lu : %.2f%%\n", i, 100 * conta_occorrenze(arr, n, i) / (double) n);
	free(arr);
	return 0;
}

char is_digit(char* str)
{
	for(size_t i = 0; i < strlen(str); i++)
		if (str[i] < '0' || str[i] > '9') return 0;
	return 1;
}

int compare (const void *a, const void *b)
{
	const int x = *(const int *) a;
	const int y = *(const int *) b;
	return x - y;
}

// restituisce -1 se x non occorre in A
int ricerca_binaria(unsigned int* arr, unsigned int dim, unsigned int x, char first)
{
	// arr[low ... high]
	int low = 0, high = dim - 1;
	int result = -1;

	while (low <= high)
	{
		unsigned int mid = (low + high)/2;
		if (x == arr[mid])
		{
			result = mid;
			if (first) high = mid - 1;
			else low = mid + 1;
		}

		else if (x < arr[mid]) high = mid - 1;
		else low = mid + 1;
	}

	return result;
}

int conta_occorrenze(unsigned int* arr, unsigned int dim, int x)
{
	unsigned int first_occ = ricerca_binaria(arr, dim, x, 1);
	if (first_occ != -1) return ricerca_binaria(arr, dim, x, 0) - first_occ + 1;
	else return -1;
}