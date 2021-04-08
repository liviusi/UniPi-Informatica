# Esercizio 3
| Tempo | Evento          | Processo in esecuzione | Coda pronti          | Note                                |
| -     | -               | -                      | -                    | -                                   |
| 0     | Arriva A        | A                      | { }                  |                                     |
| 8     | Arriva B        | A                      | { B }                |                                     |
| 10    | Termina A       | B                      | { }                  |                                     |
| 18    | Arriva C        | C                      | { B }                | B -> 8/24                           |
| 24    | Termina C       | B                      | { }                  |                                     |
| 28    | Arriva D        | D                      | { B }                | B -> 12/24                          |
| 32    | Arriva E        | D                      | { E, B }             |                                     |
| 36    | Arriva F        | D                      | { F, E, B }          |                                     |
| 38    | Termina D       | F                      | { E, B }             |                                     |
| 45    | Termina F       | E                      | { B }                |                                     |
| 56    | Termina E       | B                      | { }                  |                                     |
| 68    | Termina B       |                        |                      |                                     |
