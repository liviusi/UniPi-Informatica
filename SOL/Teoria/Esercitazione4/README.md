# Esercizio 1
In un sistema vengono generati 5 processi (A,B,C,D,E), con durate (in millisecondi) sotto specificate:
- A - 25ms;
- B - 60ms;
- C - 5ms;
- D - 15ms;
- E - 10ms.

Tutti i processi avanzano senza mai sospendersi.
Calcolare il tempo di permanenza nel sistema di ogni processo (definito come differenza tra il tempo di
completamento dell’esecuzione e il tempo di arrivo nel sistema,) e il tempo medio di permanenza dei
processi, con le seguenti politiche di scheduling
- Politica FIFO;
- Politica SJF (Shortest Job First).

Nel calcolo dei tempi di permanenza, si ignori il tempo di commutazione di contesto.

# Esercizio 2
In un sistema vengono generati 5 processi (A,B,C,D,E), con i tempi di arrivo e le durate (in
millisecondi) sotto specificate:
| Processo | Durata | Tempo di arrivo |
| - | - | - |
| A | 25 | 0 |
| B | 60 | 1 |
| C | 5 | 2 |
| D | 15 | 3 |
| E | 10 | 4 |

Tutti i processi avanzano senza mai sospendersi.
Calcolare il tempo di permanenza nel sistema di ogni processo (definito come differenza tra il tempo di
completamento dell’esecuzione e il tempo di arrivo nel sistema) con le seguenti politiche di scheduling:
- Politica FIFO;
- Politica Round-Robin con quanto di tempo pari a 5 msec.

In entrambi i casi si ignori il tempo di commutazione di contesto.

# Esercizio 3
In un sistema vengono generati 6 processi (A,B,C,D,E,F), con i tempi di arrivo, le priorità e le durate
(in millisecondi) sotto specificate:
| Processo | Tempo di Arrivo | Priorità | Durata |
|     -    |        -        |    -     |     -  |
| A | 0 | 2 | 10 |
| B | 8 | 1 | 24 |
| C | 18 | 3 | 6 |
| D | 28 | 4 | 10 |
| E | 32 | 2 | 11 |
| F | 36 | 3 | 7 |

Lo scheduling del processore avviene con una politica a prorità (che assegna il processore al processo
che ha il valore più elevato di priorità e a pari priorità, al processo arrivato per primo) e con prerilascio.
Si suppone che, una volta in esecuzione, ogni processo avanzi senza mai sospendersi.
Riempire la seguente tabella, utilizzando una riga per ogni evento che provoca la riassegnazione del
processore e specificando, in ogni riga, il processo in esecuzione e la composizione della coda pronti
subito dopo il verificarsi dell’evento.
| Tempo | Evento | Processo in esecuzione | Coda pronti | Note |
| - | - | - | - | - |
| 0 | - | - | - | - |
| 8 | - | - | - | - |
| 10 | - | - | - | - |
| 18 | - | - | - | - |
| 24 | - | - | - | - |
| 28 | - | - | - | - |
| 32 | - | - | - | - |
| 36 | - | - | - | - |
| 38 | - | - | - | - |
| 45 | - | - | - | - |
| 56 | - | - | - | - |
| 68 | - | - | - | - |

# Esercizio 4
In un sistema vengono generati 5 processi (A,B,C,D,E), con i tempi di arrivo e le durate (in millisecondi) sotto specificate:
| Processo | Durata | Tempo di arrivo |
|     -    |   -    |        -        |
| A | 45 | 0 |
| B | 55 | 13 |
| C | 15 | 21 |
| D | 5 | 25 |
| E | 15 | 29 |

Si suppone che tutti i processi avanzino senza mai sospendersi.
Lo scheduler adotta la politica Shortest Remaining Time First (SRTT), la quale seleziona per l’esecuzione il processo con
minor tempo residuo in esecuzione e prevede il prerilascio.
Calcolare il tempo di permanenza nel sistema di ogni processo (definito come differenza tra il tempo di completamento
dell’esecuzione e il tempo di arrivo nel sistema).
