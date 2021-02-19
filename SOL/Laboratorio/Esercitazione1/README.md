# Esercitazione 1 (ripasso C)
## Esercizio 1
Scrivere una funzione C che prende in input come primo argomento una stringa, come secondo argomento la lunghezza della stringa e restituisca nel terzo argomento la stessa stringa con tutti i sui caratteri in maiuscolo:
` void strtoupper(const char* in, size_t len, char* out); `

Scrivere il programma main di test per la funzione 'strtoupper' che prende la/le stringa/e da passare alla funzione come argomenti da linea di comando. Per convertire una lettera in maiuscolo si può usare 'toupper' (man 3 toupper).

## Esercizio 2
Scrivere una funzione con nome 'mystrcat' con il seguente prototipo:
` char *mystrcat(char* buffer, size_t buffer_size, char *prima, ...); `

La funzione prende come parametri un buffer, la lunghezza del buffer in bytes ed almeno una stringa (il parametro formale 'prima'). Le stringhe possono essere in numero variabile (>1). La funzione concatena tutte le stringhe passate come argomento all'interno del 'buffer' e ritorna il buffer stesso. ATTENZIONE alla gestione della memoria!

Utilizzare il seguente main per testare il codice scritto:
```C
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
 
const int REALLOC_INC=16;
 
void RIALLOCA(char** buf, size_t newsize) {
  <inserire il codice per riallocare buf alla nuova size newsize> 
}     
char* mystrcat(char *buf, size_t sz, char *first, ...) {
  <implementare il codice che se necessario chiamerà RIALLOCA>
}  
 
int main(int argc, char *argv[]) {
  if (argc < 7) { printf("troppi pochi argomenti\n"); return -1; }
  char *buffer=NULL;
  RIALLOCA(&buffer, REALLOC_INC);  // macro che effettua l'allocazione del 'buffer'
  buffer[0]='\0'; // mi assicuro che il buffer contenga una stringa
  buffer = mystrcat(buffer, REALLOC_INC, argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], NULL);
  printf("%s\n", buffer);     
  free(buffer);
  return 0;
}
```
NOTA: Che cosa può succedere se invece di ` printf("%s\n", buffer); `
si fosse scritto: 

`printf("%s\n", mystrcat(buffer, 16, "prima stringa", "seconda", "terza molto molto molto lunga", "quarta", "quinta lunga", "ultima!",NULL)); ` ?
## Esercizio 3
Non utilizzando la funzioni di libreria 'getopt' (vedere man 3 getopt), scrivere un programma che effettua il parsing della linea di comando e che riconosce le seguenti opzioni:
` -n <numero> -s <stringa> -m <altro-numero> -h. `
Il programma dovrà stampare le opzioni riconosciute con il relativo argomento. L'opzione -h non ha argomento e corrisponde al messaggio di help (i.e. usage). Se è presente l'opzione -h dovrà essere stampato solo il messaggio di di help cioè:
` nome-programma -n <numero> -s <stringa> -m <numero> -h `
Se ci sono opzioni non riconosciute queste dovranno essere stampate a video con il messaggio “opzione X non riconosciuta”. Per convertire le stringhe in interi usare la funzione di libreria la funzione strtol (vedere man 3 strtol) ad esempio nel modo seguente (ATTENZIONE: non gestisce overflow/underflow nella conversione):
```C
long isNumber(const char* s) {
   char* e = NULL;
   long val = strtol(s, &e, 0);
   if (e != NULL && *e == (char)0) return val; 
   return -1;
}
```
Testare il programma con i seguenti casi (supponiamo che l'eseguibile si chiami cmdlineparsing):
```C
cmdlineparsing -n 10 -m 11 -s 'ciao mondo' // tutte le opzioni sono riconosciute
cmdlineparsing -n 10 -h     // deve stampare il messaggio di help  
cmdlineparsing -n 10 -k 12  // k e' una opzione non riconosciuta
cmdlineparsing ----n 10 -s-s 'ciao mondo'  // deve stampare -n: 10 e -s: -s
cmdlineparsing -n10 -m11 -s'ciao mondo'  // deve stampare gli argomenti come nel primo caso
cmdlineparsing -n -m 11   // deve stampare un messaggio di errore per -n
```
