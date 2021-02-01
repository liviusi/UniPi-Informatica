# Problema dello zaino

All'interno del file *zaino.py* si trova una implementazione dell'algoritmo per risolvere il problema dello zaino col metodo Branch and Bound. 
Modificando opportunamente le variabili *valore*, *peso* e *C*
si potrà leggere lo svolgimento dell'algoritmo passaggio per passaggio.
```python
valore = np.array([11, 23, 18, 6]) # vettore dei valori degli oggetti
peso = np.array([7, 6, 3, 2]) # vettore dei pesi
C = 8 # capacità dello zaino
locked = [-1] * len(valore) # vettore delle variabili decisionali, parte da 1 (-1 := variabile libera; 0 := forzata a False; 1 := forzata a True) 
branch_and_bound(valore, peso, C, locked) # chiamata alla funzione
```
