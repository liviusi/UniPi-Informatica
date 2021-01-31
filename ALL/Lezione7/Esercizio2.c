#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int x;
    int y;
    int c;
} point_t;

/* Confronto tra le componenti c (colore) di un point_t (per qsort) */
int pnt_cmp_c(const void* a, const void* b) {
    const point_t *p1 = *(const point_t**) a;
    const point_t *p2 = *(const point_t**) b;

    return (p1->c - p2->c);
}

int query(point_t** punti, int n, int x1, int y1, int x2, int y2) {
    int i;

    int colori = 0;
    int ultimo_colore = -1;
    for(i=0;i<n;i++) {
        /* Vincolo x1 <= x <= x2 */
        if(x1 <= punti[i]->x && punti[i]->x <= x2) {
            /* Vincolo y1 <= y <= y2 */
            if(y1 <= punti[i]->y && punti[i]->y <= y2) {
                /* Se il punto cade nel rettangolo controlliamo il suo colore */
                if(punti[i]->c != ultimo_colore) {
                    colori++;
                    ultimo_colore = punti[i]->c;
                }
            }
        }
    }

    return colori;
}


int main(void) {
    int i;
    int n, m;

    scanf("%i %i", &n, &m);

    /* Allocazione dell'array di (puntatori a) punti */
    point_t** punti = malloc(n * sizeof(point_t*));

    for(i=0;i<n;i++) {
        /* Allocazione di un nuovo punto */
        point_t *pnt = malloc(sizeof(point_t));

        scanf("%i %i %i", &(pnt->x), &(pnt->y), &(pnt->c));

        punti[i] = pnt;
    }

    /* Teniamo i punti ordinati per colore */
    qsort(punti, n, sizeof(point_t*), pnt_cmp_c);

    /* Interrogazioni */
    for(i=0;i<m;i++) {
        int x1, y1, x2, y2;

        scanf("%i %i %i %i", &x1, &y1, &x2, &y2);

        printf("%i\n", query(punti, n, x1, y1, x2, y2));
    }

    /* Deallocazione della memoria */
    for(i=0;i<n;i++)
        free(punti[i]); // Ogni punto contenuto nell'array

    free(punti); // L'array di punti

    return 0;
}