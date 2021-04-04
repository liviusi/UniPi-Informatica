# Esercizio 2
Le risorse disponibili si riferiscono alla situazione subito prima dell'operazione corrispondente; con la dicitura  `Processo -> Risorsa`
si intende che il processo attende la disponibilità di quella risorsa.

| Operazione n. | Eseguibile? | Risorse disponibili | Risorse assegnate          | Processi Sospesi            |
| ------------- | ----------- | ------------------- | -------------------------- | --------------------------- |
| 1             | Sì          | { Q, R, S }         | { }                        | { }                         |
| 2             | Sì          | { R, S }            | { A: {Q} }                 | { }                         |
| 3             | Sì          | { R }               | { A: {Q}; C: {S} }         | { C -> Q }                  |
| 4             | Sì          | { R }               | { A: {Q}; C: {S} }         | { C -> Q }                  |
| 5             | Sì          | { }                 | { A: {Q}; B: {R}; C: {S} } | { C -> Q; B -> S }          |
| 6             | Sì          | { }                 | { B: {R}; C: {Q, S} }      | { B -> S }                  |
| 7             | Sì          | { }                 | { B: {R}; C: {Q, S} }      | { B -> S; A -> S }          |
| 8             | Sì          | { }                 | { B: {R}; C: {Q, S} }      | { B -> S; A -> S; C -> R }  |
| 9             | No          | { }                 | { B: {R}; C: {Q, S} }      | { B -> S; A -> S; C -> R }  |
| 10            | No          | { }                 | { B: {R}; C: {Q, S} }      | { B -> S; A -> S; C -> R }  |
| 11            | No          | { }                 | { B: {R}; C: {Q, S} }      | { B -> S; A -> S; C -> R }  |
| 12            | No          | { }                 | { B: {R}; C: {Q, S} }      | { B -> S; A -> S; C -> R }  |

- No, non può essere eseguita interamente.
- Le operazioni successive all'operazione n. 8 non sono eseguibili perché i processi A, B, C sono sospesi.
- Sì, provoca uno stallo.
- Si raggiunge con l'operazione n. 8.
