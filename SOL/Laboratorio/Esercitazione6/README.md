# Esercitazione 6

## Esercizio 1
Realizzare una shell **rudimentale** (dummyshell) che legge un comando con eventuali parametri dallo standard input e ne invoca l'esecuzione utilizzando una funzione di libreria della famiglia `exec*`. La shell deve terminare se viene digitato il comando 'exit'. Il formato dei comandi accettati dalla shell e' molto semplice e non non prevede metacaratteri, redirezione, pipe, lettura di variabili d'ambiente, etc…

## Esercizio 2
Scrivere un programma C che appena attivato va immediatamente in background, attende per X secondi (eseguendo il programma `/bin/sleep` con una chiamata ad una exec*) dove X e' l'argomento del programma e poi stampa il suo pid, il pid del padre e quindi termina.

## Esercizio 3
Un processo zombie è un processo terminato ma che ancora possiede delle risorse ('pid' e PCB) che non possono essere liberate perché il processo padre, non ancora terminato, potrebbe decidere di leggere lo exit status del processo figlio.

Scrivere un programma che prende un intero N come argomento e crea N processi zombies. Lanciare il programma in background e visualizzare gli zombies con il comando bash `ps -A -ostat,pid,ppid | grep Z`.

## Esercizio 4
Scrivere un programma C che prende come argomento un intero N>1. L'intero N rappresenta il numero di processi figli che devono essere creati in totale. I processi vanno creati come segue: il processo main stampa il suo pid e crea un figlio attendendone quindi la sua terminazione, il processo figlio a sua volta stampa il suo pid e crea un altro figlio attendendone la terminazione e cosi' via fino ad ottenere N processi figli in tutto. L'output formattato che si richiede che venga stampato sullo standard output e' il seguente:
```
./family 4
---- 20894: creo un processo figlio
--- 20895: creo un processo figlio
-- 20896: creo un processo figlio
- 20897: creo un processo figlio
20898: sono l'ultimo discendente
20898: terminato con successo
- 20897: terminato con successo
-- 20896: terminato con successo
--- 20895: terminato con successo
---- 20894: terminato con successo
```

## Esercizio 5
Completare il codice seguente in modo che il programma calcoli i primi 13 numeri di Fibonacci utilizzando per ogni chiamata `doFib` un processo distinto.

```C
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/* 
 * Calcola ricorsivamente il numero di Fibonacci dell'argomento 'n'.
 * La soluzione deve effettuare fork di processi con il vincolo che 
 * ogni processo esegua 'doFib' al più una volta.  
 * Se l'argomento doPrint e' 1 allora la funzione stampa il numero calcolato 
 * prima di passarlo al processo padre. 
 */
static void doFib(int n, int doPrint);

int main(int argc, char *argv[])
{
	// questo programma puo' calcolare i numeri di Fibonacci solo fino a 13.  
	const int NMAX=13;
	int arg;

	if(argc != 2)
	{
		fprintf(stderr, "Usage: %s <num>\n", argv[0]);
		return EXIT_FAILURE;
	}
	arg = atoi(argv[1]);
	if(arg <= 0 || arg > NMAX)
	{
		fprintf(stderr, "num deve essere compreso tra 1 e 13\n");
		return EXIT_FAILURE;
	}   
	doFib(arg, 1);
	return 0;
}
```
