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
