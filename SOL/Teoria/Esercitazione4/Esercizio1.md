# Esercizio 1

**Politica FIFO**:
| Processo | Tempo di inizio esecuzione [ms] | Tempo di fine esecuzione [ms] | Tempo di permanenza nel sistema [ms] |
| -------- | ------------------------------- | ----------------------------- | ------------------------------------ |
|    A     |          0                      |         25                    |       25                             |
|    B     |            25                   |             85                |        85                            |
|      C   |            85                   |           90                  |        90                            |
|       D  |             90                  |         105                   |       105                            |
|     E    |              105                |              115              |         115                          |

*Tempo medio* [ms]: (25 + 85 + 90 + 105 + 115) / 5 = 84.

**Politica SJF**: 
| Processo | Tempo di inizio esecuzione [ms] | Tempo di fine esecuzione [ms] | Tempo di permanenza nel sistema [ms] |
| -------- | ------------------------------- | ----------------------------- | ------------------------------------ |
|      A   |         30                      |            55                 |           55                         |
|      B   |         55                      |            115                |           115                        |
|      C   |         0                       |            5                  |            5                         |
|      D   |          15                     |            30                 |           30                         |
|      E   |           5                     |           15                  |            15                        |

*Tempo medio* [ms]: (55 + 115 + 5 + 30 + 15) / 5 = 44.
