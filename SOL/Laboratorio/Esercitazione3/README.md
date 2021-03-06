# Esercitazione 3
## Esercizio 1: libtokenizer.a
Inserire in un file 'tokenizer.h' la dichiarazione di due funzioni 'tokenizer' e 'tokenizer_r' (vedere la soluzione proposta nell'Esercitazione 2). 'tokenizer' ha la stessa interfaccia di 'tokenizer_r' ma mentre la prima usa al suo interno 'strtok', la seconda usa 'strtok_r'. Inserire nel file 'tokenizer_lib.c' l'implementazione delle due funzioni (cioè la loro definizione). Creare quindi una librerie statica 'libtokenizer.a' che offre l'implementazione delle due funzioni. Scrivere un programma main (es. tokenizer_main.c) che utilizza una delle due (o entrambe le) funzioni, il programma includerà il file 'tokenizer.h' e verrà linkato alla libreria statica 'libtokenizer.a'.

## Esercizio 2: libtokenizer.so
Come nell'Esercizio 1 però invece che una libreria statica creare una libreria condivisa ('libtokenizer.so'). Verificarne il corretto caricamento in memoria della libreria spostando il file 'libtokenizer.so' in una directory differente da quella corrente (es. ~/mylibdir). Provare ad usare sia l'opzione del linker -Wl, -rpath, che la variabile d'ambiente per il dynamic linker/loader LD_LIBRARY_PATH.

## Esercizio 3: macro
Dato il seguente codice main C:

```C
#define dimN 16
#define dimM  8
int main() {
    long *M = malloc(dimN*dimM*sizeof(long));
    CHECK_PTR_EXIT(M, "malloc"); 
    for(size_t i=0;i<dimN;++i)
	for(size_t j=0;j<dimM;++j)			
	    ELEM(M,i,j) = i+j;    
    
    PRINTMAT(M, dimN, dimM);
    free(M);
    return 0;
}
```
scrivere le 3 macros CHECK_PTR_EXIT, ELEM e PRINTMAT. 'CHECK_PTR_EXIT' prende due argomenti, un puntatore ed una stringa. La macro controlla che il primo argomento sia diverso da 'NULL'. Solo se il puntatore è 'NULL' stampa utilizzando la funzione di librerie perror (man 3 'perror') la stringa passata come secondo argomento prima di uscire dal programma con EXIT_FAILURE (man 3 'exit'). 'PRINTMAT' stampa la matrice passata come primo argomento. Al suo interno PRINTMAT usa la macro ELEM per accedere agli elementi della matrice (es. ELEM(M,3,4) equivale ad accedere all'elemento M[3][4] della matrice M – attenzione però che la matrice è allocata in modo contiguo in memoria e per righe).

Esempio di output richiesto:
```
Stampo la matrice M:
   0    1    2    3    4    5    6    7 
   1    2    3    4    5    6    7    8 
   2    3 ....
```

## Esercizio 4: somma da
Scrivere un programma C che implementa una funzione somma con la seguente segnatura:` int somma(int x); `.
La funzione 'somma' aggiunge l'intero x letto dallo standard input (con scanf o fscanf) al valore calcolato fino a quel momento, quindi ritorna la somma parziale. Il numero di valori da leggere dallo standard input viene passato come argomento del main. Il valore iniziale della somma deve essere 'INIT_VALUE' che viene definito a compilazione utilizzando il flag del preprocessore -D (es. -DINIT_VALUE=100). Il programma non deve fare uso di variabili globali.

Valutare se la funzione somma così come definita è rientrante ed eventualmente fornire una versione rientrante 'somma_r'.

## Esercizio 5
Considerare il seguente programma C (prog.c):
```C
int main(int argc, char *argv[]) {
  int opt;
  while ((opt = getopt(argc,argv, ":n:m:h")) != -1) {
    switch(opt) {
    case 'n':
    case 'm':printf("l'argomento di '-%c' e' '%s'\n",opt,optarg); break;
    case 'h':printf("'-h' non ha argomenti\n"); break;
    case ':':printf("'-%c' richiede un argomento\n", optopt); break;
    case '?':printf("'-%c' non e' riconosciuta\n", optopt);
    default:;
    }
  }  
  return 0;
}
```
Modificare il contenuto del file prog.c senza aggiungere alcun #include file e senza modificare la funzione 'main', in modo che il programma possa essere compilato col comando seguente:
` gcc -Wall -ansi -pedantic prog.c -o prog `.
