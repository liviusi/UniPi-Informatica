# Esercitazione7

## Esercizio 1 (buffer con capacità limitata)
Scrivere un programma C con due threads, un produttore (P) ed un consumatore (C). Il thread P genera, uno alla volta, una sequenza di numeri inserendoli in un buffer di una sola posizione condiviso con il thread C. Il thread consumatore estrae i numeri dal buffer e li stampa sullo standard output. Se il buffer e' pieno P attende che C consumi il dato, analogamente se il buffer e' vuoto C attende che P produca un valore da consumare.

## Esercizio 2 (filosofi a cena)
N filosofi siedono ad una tavola con un piatto di spaghetti davanti ed una forchetta alla loro destra ed una alla loro sinistra. Per mangiare gli spaghetti un filosofo ha bisogno di entrambe le forchette vicine (!). Ogni filosofo è impegnato ininterrottamente in una sequenza di 3 attivita': meditare, cercare di acquisire le forchette e mangiare. Scrivere un programma C che attivi N threads filosofi (con N>=5), ognuno dei quali esegue il ciclo descritto in precedenza per 100 volte. La meditazione e la fase in cui il filosofo mangia deve essere implementata con un ritardo variabile usando ad esempio la chiamata di sistema 
`nanosleep` e la funzione `rand_r()`.
SUGGERIMENTO: Per evitare il deadlock nell'aquisizione delle forchette, una possibile soluzione è definire un ordinamento opportuno per l'acquisizione delle forchette da parte di ogni filosofo.

## Esercizio 3 (pipeline concorrente)
Scrivere un programma C che implementa una pipeline di tre threads. Il primo thread legge una riga alla volta da un file testuale (il cui nome e' passato come argomento al programma) ed invia al secondo thread ogni riga letta dal file. Il secondo thread “tokenizza” le parole dalla riga ricevuta dal primo thread (considerare come separatore solo lo spazio) ed invia al terzo thread le parole. Il terzo thread stampa tutte le parole sullo standard output. I tre stadi della pipeline devono lavorare in modo concorrente come in una “catena di montaggio”, il buffer di comunicazione tra due stadi della pipeline deve essere implementata con una coda FIFO (la scelta se usare una coda di capacita bounded o unbounded e' lasciata allo studente).