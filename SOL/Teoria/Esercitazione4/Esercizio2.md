# Esercizio 2

**Politica FIFO**:
| Processo | Tempo di inizio esecuzione [ms] | Tempo di fine esecuzione [ms] | Tempo di permanenza nel sistema |
|     -    |                  -              |               -               |              -                  |
|    A     |             0                   |             25                |            25                   |
|    B     |             25                  |             85                |            84                   |
|    C     |             85                  |              90               |            88                   |
|    D     |             90                  |             105               |            102                  |
|    E     |             105                 |             115               |            111                  |

**Politica Round-Robin**:
| Tempo [ms] | Processo in esecuzione | Processi in attesa                                                                         | Processi completati   |
|    -       |          -             |         -                                                                                  |         -             |
|    0       |          A             |        { }                                                                                 |        { }            | 
|    5       |         B              |   { A -> 5/25 }                                                                            |        { }            |
|    10      |          C             | { A -> 5/25;  B -> 5/60 }                                                                  |        { }            | 
|    15      |          D             | { A -> 5/25; B -> 5/60 }                                                                   |    { **C** }          | 
|    20      |          E             | { A -> 5/25; B -> 5/60; D -> 5/15 }                                                        |        { C }          | 
|    25      |          A             | { A -> 5/25; B -> 5/60; D -> 5/15; E -> 5/10 }                                             |        { C }          | 
|    30      |          B             | { A -> 10/25; B -> 5/60; D -> 5/15; E -> 5/10 }                                            |        { C }          | 
|    35      |          D             | { A -> 10/25; B -> 10/60; D -> 5/15; E -> 5/10 }                                           |         { C }         | 
|    40      |          E             | { A -> 10/25; B -> 10/60; D -> 10/15; E -> 5/10 }                                          |       { C }           | 
|    45      |          A             | { A -> 10/25; B -> 10/60; D -> 10/15 }                                                     |   { C, **E** }        | 
|    50      |          B             | { A -> 15/25; B -> 10/60; D -> 10/15 }                                                     |   { C, E }            | 
|    55      |          D             |  { A -> 15/25; B -> 15/60; D -> 10/15 }                                                    |   { C, E }            |
|    60      |          A             |  { A -> 15/25; B -> 15/60 }                                                                |   { C, **D**, E }     |
|    65      |          B             |  { A -> 20/25; B -> 15/60 }                                                                |   { C, D, E }         |
|    70      |          A             |  { A -> 20/25; B -> 20/60 }                                                                |   { C, D, E }         |
|    75      |          B             |  { }                                                                                       | { **A**, C, D, E }    |
| [75; 115[  |          B             |  { }                                                                                       |   { A, C, D, E }      |
| 115        |         { }             |  { }                                                                                      | { A, **B**, C, D, E } |
