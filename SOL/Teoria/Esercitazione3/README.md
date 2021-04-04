# Esercizio 1
Dati tre processi A,B,C che osservano il paradigma di “possesso e attesa” e tre risorse singole Q, R, S, utilizzabili in
mutua esclusione e senza possibilità di prerilascio, supponiamo che il gestore assegni le risorse al processo richiedente
alla sola condizione che la risorsa sia disponibile. Inizialmente tutte le risorse sono disponibili.
Si considerino le seguenti sequenze di richieste e rilasci:

**Sequenza S1**:
1. A richiede Q;
2. C richiede S;
3. C richiede Q;
4. B richiede R;
5. B richiede S;
6. A rilascia Q;
7. A richiede S;
8. C richiede R.

**Sequenza S2**:
1. A richiede R;
2. B richiede Q,
3. C richiede S;
4. C richiede R;
5. A richiede Q;
6. B rilascia Q;
7. B richiede S;
8. A rilascia R.

Queste sequenze provocano stallo? Motivare le risposte rappresentando nel grafo lo stato (assegnazioni e richieste
pendenti) al termine di ciascuna sequenza.

# Esercizio 2
Dati tre processi A,B,C che osservano il paradigma di “possesso e attesa” e tre risorse singole Q, R, S, utilizzabili in
mutua esclusione e senza possibilità di prerilascio, supponiamo che il gestore assegni le risorse al processo richiedente
alla sola condizione che la risorsa sia disponibile. Inizialmente tutte le risorse sono disponibili.
Supponendo che, per effetto della concorrenza, le richieste e i rilasci vengano intercalate nel modo seguente
1. A richiede Q;
2. C richiede S;
3. C richiede Q;
4. B richiede R;
5. B richiede S;
6. A rilascia Q;
7. A richiede S;
8. C richiede R;
9. A rilascia S;
10. C rilascia Q;
11. B rilascia R;
12. C rilascia S.

mostrare come si evolve il sistema utilizzando la tabella riportata nello schema di soluzione, e rispondere alle seguenti
domande:
- la precedente sequenza di operazioni può essere eseguita completamente?
- eventualmente, quali operazioni non possono essere eseguite e perché?
- si raggiunge uno stallo?
- eventualmente, con quale operazione si raggiunge lo stallo? 

# Esercizio 3
In un sistema con risorse R1, R2, R3, R4 e R5, tutte con molteplicità 2, sono presenti i processi P1, P2 e P3 che
inizialmente non possiedono risorse e successivamente avanzano senza interagire reciprocamente e alternandosi nello
stato di esecuzione con velocità arbitrarie.
Nel corso della propria esistenza, ciascun processo esegue una propria sequenza di richieste, che si intercalano in modo
arbitrario con quelle degli altri processi. Dopo aver ottenuto e utilizzato le risorse che richiede, ogni processo termina
rilasciando tutte le risorse ottenute.
Si considerino, in alternativa, le sequenze di richieste (semplici o multiple) sotto riportate:

**Sequenza 1**:

| Processo | Prima richiesta | Seconda richiesta | Terza richiesta | Quarta richiesta | Terminazione |
| -------- | --------------- | ----------------- | --------------- | ---------------- | ------------ |
| P1 | 2 istanze di R1 | 1 istanza di R3 | 1 istanza di R4 | 1 istanza di R5 | Rilascia |
| P2 | 2 istanze di R2 | 1 istanza di R3 | 2 istanze di R4 | 1 istanza di R3 | Rilascia |
| P3 | 2 istanze di R1 | 1 istanza di R3 | 1 istanza di R4 | 2 istanze di R5 | Rilascia |

**Sequenza 2**:

| Processo | Prima richiesta | Seconda richiesta | Terza richiesta | Quarta richiesta | Terminazione |
| -------- | --------------- | ----------------- | --------------- | ---------------- | ------------ |
| P1 | 2 istanze di R1 | 1 istanza di R3 | 1 istanza di R4 | 1 istanza di R5 | Rilascia |
| P2 | 2 istanze di R2 | 1 istanza di R3 | 2 istanze di R4 | 1 istanza di R5 | Rilascia |
| P3 | 2 istanze di R1 | 1 istanza di R2 | 1 istanza di R3 | 2 istanze di R5 | Rilascia |

Per ogni sequenza, si chiede se i processi evitano la possibilità di stallo e la motivazione della risposta.

# Esercizio 4
Un sistema con processi A, B, C, D, E e risorse dei tipi R1, R2, R3, R4, ha raggiunto lo stato mostrato nelle tabelle
seguenti, che è uno stato sicuro:

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

Successivamente, i processi E e C eseguono in sequenza le seguenti richieste:
1. E richiede 2 istanze di R4 (richiesta multipla: deve essere soddisfatta integralmente);
2. C richiede 1 istanza di R3.

Il gestore delle risorse applica l’algoritmo del banchiere per evitare lo stallo. Verificare se il gestore assegna le risorse
richieste.

# Esercizio 5

Un sistema con processi A, B, C, D e risorse dei tipi R1, R2, R3, R4, rispettivamente di molteplicità [5, 6, 3, 6] adotta
nei confronti dello stallo la politica di riconoscimento ed eliminazione. Ad un dato tempo t, quando le assegnazioni ai
processi e la disponibilità delle risorse hanno i valori mostrati nella prima e nella terza parte della tabella, i processi
avanzano nel modo seguente:
- A richiede 1 istanza di R4 e si sospende;
- B richiede, con richiesta multipla, 2 istanze di R3 e si sospende;
- C richiede 1 istanza di R4 e si sospende;
- D richiede 1 istanza di R4 e si sospende.

Dopo l’ultima richiesta i processi hanno pendenti le richieste mostrati nella seconda parte della tabella e si raggiunge
uno stallo: infatti i processi dell’insieme {A, B, C, D} sono in attesa di risorse nell’insieme {R3, R4} e tutte le risorse
nell’insieme {R3, R4} sono assegnata a processi nell’insieme {A, B, C, D}.

**Assegnazione attuale**:
| Processo | R1 | R2 | R3 | R4 | 
| -------- | -- | -- | -- | -- |
| A        | 2  |    |    | 3  |
| B        | 1  | 1  |    |    |
| C        |    |    | 2  | 1  |
| D        |    | 3  |    | 2  |

**Richiesta pendente**:
| Processo | R1 | R2 | R3 | R4 | 
| -------- | -- | -- | -- | -- |
| A        |    |    |    | 1  |
| B        |    |    | 2  |    |
| C        |    |    |    | 1  |
| D        |    |    |    | 1  | 

**Vettore molteplicità**:
| R1 | R2 | R3 | R4 |
| -- | -- | -- | -- |
| 5  | 6  | 3  | 6  |

**Vettore disponibilità**:
| R1 | R2 | R3 | R4 |
| -- | -- | -- | -- |
| 2  | 2  | 1  | 0  |

Al fine di eliminare lo stallo si eseguono, in alternativa, le seguenti azioni:
- il processo A viene forzato a rilasciare 1 risorsa di tipo R1, che richiederà dopo l’eventuale riattivazione;
- il processo C viene forzato a rilasciare 1 risorsa di tipo R4, che richiederà dopo l’eventuale riattivazione.

Lo stallo si considera eliminato se valgono le seguenti due condizioni:
- dopo il forzato rilascio si può soddisfare la richiesta pendente di almeno uno dei processi sospesi, che pertanto
viene riattivato, e nel nuovo stato non si ha attesa circolare;
- esiste la possibilità di soddisfare tutte le ulteriori richieste che ciascun processo effettua dopo la riattivazione
conseguente al soddisfacimento della richiesta pendente (nel caso del processo che è stato forzato a rilasciare una
risorsa, le ulteriori richieste comprendono anche quella della risorsa che è stata rilasciata). Queste richieste, che non
sono note a priori, sono riportate nella terza parte della tabella inserita nello schema di soluzione. Si noti che la
possibilità di soddisfare le ulteriori richieste può verificarsi o no, a seconda delle velocità di avanzamento dei
processi. Per verificare la possibilità di soddifare le richieste successive, si consideri la sequenza di avanzamento
più favorevole.


Si chiede se le ipotesi a) oppure b) permettono di eliminare lo stallo.
