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
3. C richiede Q
4. B richiede R;
5. B richiede S;
6. A rilascia Q;
7. A richiede S;
8. C richiede R;
9. A rilascia S;
10. C rilascia Q
11. B rilascia R
12. C rilascia S

mostrare come si evolve il sistema utilizzando la tabella riportata nello schema di soluzione, e rispondere alle seguenti
domande:
- la precedente sequenza di operazioni può essere eseguita completamente?
- eventualmente, quali operazioni non possono essere eseguite e perché?
- si raggiunge uno stallo?
- eventualmente, con quale operazione si raggiunge lo stallo? 
