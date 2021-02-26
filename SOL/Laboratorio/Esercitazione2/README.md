# Esercitazione 2
## Esercizio 1: ''getopt''
Scrivere un programma C che prende in ingresso 4 opzioni da linea di comando codificate con '-n', '-m', '-o', '-h' nel modo seguente:

` nome-programma -n <num. intero> -m <num. intero> -o <stringa> -h `
Il programma dovrà stampare sullo standard output le opzioni riconosciute valide con il relativo argomento. L'opzione '-h' non ha argomento e corrisponde al messaggio di uso. Effettuare il parsing della command line utilizzando la funzione di libreria getopt (man 3 getopt).

## Esercizio 2: puntatori a funzioni
Estendere l'esercizio 1 associando ad ogni opzione del main una funzione che ne faccia la gestione e la stampa sullo standard output. Ad esempio, la gestione dell'opzione '-n' viene fatta dalla funzione 'arg_n(const char*)' la quale convertirà l'argomento associato all'opzione ad intero e farà la stampa sullo standard output, ritornado -1 in caso di errore e 0 in caso di successo. Memorizzare tali funzioni in un vettore con nome 'V'. Utilizzare completando il seguente frammento di codice main.

```C 
int main(int argc, char* argv[]) {
 
  ...  // controllo di argc ed inizializzazione del vettore V con i puntatori a funzione
 
  int opt;
  while ((opt = getopt(argc,argv, "n:m:o:h")) != -1) {
    switch(opt) {
    case '?': { 
       <gestione parametro non riconosciuto> 
    } break;
    default:
     // invocazione della funzione di gestione passando come parametro l'argomento restituito da getopt
     if (V[opt%4]( (optarg==NULL ? argv[0] : optarg) ) == -1) {
      <gestione errore>
     }
    }
  }
  return 0; 
}
```
## Esercizio 3: funzioni rientranti
Si consideri il seguente programma:
```C
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
	fprintf(stderr, "use: %s stringa1 stringa2\n", argv[0]);
	return -1;
    }
    char* token1 = strtok(argv[1], " ");

    while (token1) {
	printf("%s\n", token1);
	char* token2 = strtok(argv[2], " ");
	while(token2) {
	    printf("%s\n", token2);
	    token2 = strtok(NULL, " ");
	}
	token1 = strtok(NULL, " ");
    }
    return 0;
}
```
Riscrivere il programma precedente (che produce un output non corretto) utilizzando la chiamata di libreria strtok_r al posto di strtok.

NOTA: se si utilizza lo standard ISO 99 o 11 (-std=c99/11), per evitare i warnings del tipo “implicit declaration of function X” aggiungere la seguente opzione di compilazione “-D_POSIX_C_SOURCE=200112L”, oppure dichiarare il prototipo della funzione (copiando la segnatura dal man della funzione), oppure inserire la seguente define **prima del primo include**:
` #define _POSIX_C_SOURCE 200112L `

## Esercizio 4: numeri random
Generare N numeri casuali interi nell'intervallo [K1,K2[ utilizzando la funzione `rand_r()`. N, K1 e K2 sono passati come argomenti del main opzionali, e se non forniti dall'utente assumono valori di default sulla base di opportune #define (es. N=1000 K1=100 K2=120). Il programma deve restituire il numero di occorrenze di ciascun intero i nell'intervallo [K1,K2[e stamparle sullo standard output. Un esempio di output prodotto considerando K1=0 e K2=10, è il seguente:
```
Occorrenze di:
0  : 10.25% 
1  :  9.97% 
2  :  9.48% 
3  :  9.77% 
4  : 10.19% 
5  : 10.93% 
6  :  9.80% 
7  :  9.93% 
8  : 10.00% 
9  :  9.68%
```
