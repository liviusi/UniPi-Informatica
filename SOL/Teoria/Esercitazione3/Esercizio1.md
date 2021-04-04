# Esercizio 1

**Sequenza S1**:
1. A ottiene Q;
2. C ottiene S;
3. C non ottiene Q, viene sospeso;
4. B ottiene R;
5. B non ottiene S, viene sospeso;
6. A rilascia Q, C ottiene Q (non è più sospeso);
7. A non ottiene S, viene sospeso;
8. C non ottiene R, viene sospeso.

S1 porta allo stallo.

**Sequenza S2**:
1. A ottiene R;
2. B ottiene Q,
3. C ottiene S;
4. C non ottiene R, viene sospeso;
5. A non ottiene Q, viene sospeso;
6. B rilascia Q, A ottiene Q (non è più sospeso);
7. B non ottiene S, viene sospeso;
8. A rilascia R, C ottiene R (non è più sospeso).

S2 non porta allo stallo.
