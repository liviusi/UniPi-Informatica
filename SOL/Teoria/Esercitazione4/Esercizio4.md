# Esercizio 4

| Tempo |   Evento         | Processo in esecuzione |                                             Coda pronti |
|   -   |         -        |                  -     |                         -                               |
|   0   | Arriva A         |          A             |                                                         |
|   13  | Arriva B         |           A            |    { B -> 0/55 }                                        |
|   21  | Arriva C         |         C              |   { A -> 21/45, B -> 0/55 }                             |
|   25  | Arriva D         |          D             |   { C -> 4/15, A -> 21/45, B -> 0/55 }                  |
|    29 | Arriva E         |          D             |   { C -> 4/15, E -> 0/15, A -> 21/45, B -> 0/55 }       |
|   30  | Termina D        |          C             |   { E -> 0/15, A -> 21/45, B -> 0/55 }                  |
|  41   | Termina C        |          E             |   { A -> 21/45, B -> 0/55 }                             |
|  56   | Termina E        |          A             |   { B -> 0/55 }                                         |
|  80   | Termina A        |          B             |                                                         |
|  135  | Termina B        |                        |                                                         |
