# Esercizio 4
Lo stato raggiunto dopo l'assegnazione di 2 istanze di R4 al processo E risulta essere:

**Assegnazione attuale**:
| Processo | R1 | R2 | R3 | R4 | 
| -------- | -- | -- | -- | -- |
| A        | 1  | 1  | 1  | 2  |
| B        | 2  |    |    | 1  |
| C        |    | 3  | 2  |    |
| D        | 1  | 1  |    |    |
| E        | 2  | 1  |    | 2  |

**Esigenza attuale**:
| Processo | R1 | R2 | R3 | R4 | 
| -------- | -- | -- | -- | -- |
| A        | 3  | 5  | 3  | 4  |
| B        | 2  | 2  |    | 1  |
| C        | 3  | 1  | 1  | 2  |
| D        | 4  | 5  | 4  | 3  | 
| E        |    |    |    | 1  |

**Vettore molteplicità**:
| R1 | R2 | R3 | R4 |
| -- | -- | -- | -- |
| 6  | 7  | 4  | 6  |

**Vettore disponibilità**:
| R1 | R2 | R3 | R4 |
| -- | -- | -- | -- |
| 0  | 1  | 1  | 1  |

Si verifica se lo stato risulta sicuro:
- E può terminare, il vettore disponibilità diventa [2, 2, 1, 3];
- B può terminare, il vettore disponibilità diventa [4, 2, 1, 4];
- C può terminare, il vettore disponibilità diventa [4, 5, 3, 4];
- A può terminare, il vettore disponibilità diventa [5, 6, 4, 6];
- D può terminare, il vettore disponibilità diventa [6, 7, 4, 6].

Lo stato è dunque sicuro, la richiesta viene accettata.

Si considera adesso lo stato dopo la richiesta del processo C di una istanza di R3.

**Assegnazione attuale**:
| Processo | R1 | R2 | R3 | R4 | 
| -------- | -- | -- | -- | -- |
| A        | 1  | 1  | 1  | 2  |
| B        | 2  |    |    | 1  |
| C        |    | 3  | 3  |    |
| D        | 1  | 1  |    |    |
| E        | 2  | 1  |    | 2  |

**Esigenza attuale**:
| Processo | R1 | R2 | R3 | R4 | 
| -------- | -- | -- | -- | -- |
| A        | 3  | 5  | 3  | 4  |
| B        | 2  | 2  |    | 1  |
| C        | 3  | 1  |    | 2  |
| D        | 4  | 5  | 4  | 3  | 
| E        |    |    |    | 1  |

**Vettore molteplicità**:
| R1 | R2 | R3 | R4 |
| -- | -- | -- | -- |
| 6  | 7  | 4  | 6  |

**Vettore disponibilità**:
| R1 | R2 | R3 | R4 |
| -- | -- | -- | -- |
| 0  | 1  | 0  | 1  |

Si verifica se lo stato risulta sicuro:
- E può terminare, il vettore disponibilità diventa [2, 2, 0, 3];
- B può terminare, il vettore disponibilità diventa [4, 2, 0, 4];
- C può terminare, il vettore disponibilità diventa [4, 5, 3, 4];
- A può terminare, il vettore disponibilità diventa [5, 6, 4, 6];
- D può terminare, il vettore disponibilità diventa [6, 7, 4, 6].

Lo stato è dunque sicuro, la richiesta viene accettata.
