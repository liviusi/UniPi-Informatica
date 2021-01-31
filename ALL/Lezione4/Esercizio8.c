#include <time.h>
#include <stdlib.h>
#include <stdio.h>

void swap(int* a, int* b){
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

/* 
  !!!!!!!!!!!! D A  F A R E !!!!!!!!!!!!
  sx e dx sono le posizioni del primo e dell'ultimo elemento dell'array mentre 
  px è la posizione dell'elemento perno.
  La funzione deve restituire la posizione del perno dopo che gli elementi sono 
  stati partizionati.
*/
void distribuzione(int a[], int sx, int px, int dx, int *minori, int *maggiori) { 
    int pivot = a[px];
    int i = sx - 1;
    int j = sx;
    int k = dx;

    swap(&a[px], &a[dx]);
    px = dx;

    while ( j < k )
    {
        if ( a[j] < pivot )
        {
            i++;
            swap(&a[i], &a[j]);
            j++;
        }
        else if ( a[j] == pivot )
            j++;
        else
        {
            k--;
            swap(&a[j], &a[k]);
        }
    }

    swap(&a[j], &a[dx]);
    *minori = i;
    *maggiori = k + 1;
}

void quicksort( int a[], int sx, int dx ) {
  
  int pivot, minori, maggiori;
  if( sx < dx ) {
    /* DA IMPLEMENTARE: scelta del pivot. Scegliere una posizione a caso tra sx e dx inclusi. */
    pivot = rand()%(dx - sx + 1) + sx;

    distribuzione(a, sx, pivot, dx, &minori, &maggiori); /* separa gli elementi minori di a[pivot] 
					        da quelli maggiori o uguali */
    /* Ordina ricorsivamente le due metà */
    quicksort(a, sx, minori);
    quicksort(a, maggiori, dx);
    
  }

}

/* Lettura di un array di interi da input. 
Il primo elemento è la lunghezza dell'array */
int legge(int **a, int *len) {

  int i;
  scanf("%d", len);
  if(*len <= 0) return 1;

  *a = (int *) malloc(*len * sizeof(int));
  if(*a == NULL) return 1;

  for( i = 0; i < *len; i++ ) 
	  scanf("%d", (*a)+i);
  
  return 0;

}

int main() {
  
  int i, n, *A;

  if( legge(&A, &n)) return 1;

  srand(time(NULL));
  quicksort(A, 0, n-1);
  
  /* Stampa l'array ordinato */
  for( i = 0; i < n; i++ ) 
    printf("%d ", A[i]);
	
  return 0;
}