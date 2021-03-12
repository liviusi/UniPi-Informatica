# Esercitazione 4
## Esercizio 1
Scrivere un programma C che legge il file testuale “/etc/passwd” e scrive in un file testuale la lista dei 'login name' del sistema, uno per riga. Il nome del file testuale di output viene passato come unico argomento al programma.
Il formato del file “/etc/passwd” è descritto nella sezione 5 del manuale entry 'passwd' (`man 5 passwd`) :
“/etc/passwd contains one line for each user account, with seven fields delimited by colons (”:“).”
Il primo campo è il 'login name'.
Suggerimento: usare `fgets` per leggere le righe del file; usare strchr per trovare la prima occorrenza di ':' nella stringa letta dal file.

## Esercizio 2
Scrivere un programma che prende in ingresso un intero 'N' ('N' e' piccolo, es. <=512), alloca in memoria una matrice di NxN elementi di tipo float in modo che siano contigui in memoria. Inizializzare tale matrice (chiamata ad esempio M1) nel modo seguente M1(i,j) = (i+j)/2.0. Fare quindi il salvataggio in un file della matrice M1 sia in formato **binario** che in formato **testuale** in modo da ottenere due file separati rispettivamente con nome 'mat_dump.dat' per il formato binario, e 'mat_dump.txt' per quello testuale. Chiudere i file creati.
Scrivere un secondo programma (oppure estendere quello precedente) che prende in ingresso i nomi dei due file creati in precedenza e l'intero 'N', legge la matrice sia dal file 'mat_dump.dat' che dal file 'mat_dump.txt' memorizzandone il contenuto in due matrici distinte (ad esempio M1 ed M2). Al termine dell'operazione di lettura, effettuare il confronto delle due matrici con la funzione 'confronta' che deve essere implementata in modo “generico” cioe' deve prendere come argomenti un puntatore a funzione con tipo opportuno, i puntatori alle due matrici da confrontare e la size delle matrici. Il programma deve stampare l'esito del confronto. Testare il programma passando alla funzione 'confronta' la funzione di libreria `memcmp` (`man 3 memcmp`).

Notare la differenza di size tra 'mat_dump.dat' e 'mat_dump.txt'.
Suggerimento: per leggere e scriver un file binario utilizzare rispettivamente le funzioni di libreria fread ed fwrite. Per scrivere il file testuale usare la chiamata `fprintf`.

## Esercizio 3
Scrivere un programma che implementa una versione semplificata del comando Unix `wc` (word count) limitatamente alle opzioni -l e -w (`man 1 wc`). Usare `getopt` per il parsing degli argomenti.

Se non vengono passate opzioni, il programma stampa sia il numero di linee che il numero di parole del/dei file i cui nomi sono passati come argomento (mywc [-l -w] file1 [file2 file3 ….]).
Suggerimento: scrivere una funzione che conta le parole di una stringa utilizzando le funzioni di utilità della libreria standard come `isspace`, `iscntrl` (`man 3 isspace`).

## Esercizio 4
Scrivere un semplice Makefile che permette di compilare tutti i file soluzione degli esercizi di questo assegnamento modificando/estendendo opportunamente il seguente file:
```Makefile
CC		=  gcc
CFLAGS	        += 
TARGETS		= 

.PHONY: 
.SUFFIXES: .c .h

all		:

clean		: 
	-rm -f $(TARGETS)
```