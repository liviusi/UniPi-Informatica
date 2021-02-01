# Algoritmo del simplesso primale

All'interno del file *primale.py* si trova una implementazione dell'algoritmo del simplesso primale per sistemi di equazioni lineari in due variabili. 
Modificando opportunamente le variabili *A*, *b*, *c* e *B*
si potrà leggere lo svolgimento dell'algoritmo passaggio per passaggio.
```python
A = np.array([[1, 0], [1, 1], [-1, 0], [0, -1]]) # matrice dei coefficienti
b = np.array([2, 3, 0, 0]) # matrice dei termini noti
c = np.array([2, 1]) # matrice dei coefficienti della funzione obiettivo
B = np.array([3, 4]) # base di partenza - gli indici partono da 1 anziché da 0
simplesso_primale(A, b, c, B) # chiamata alla funzione
```

# Algoritmo del simplesso duale

All'interno del file *duale.py* si trova una implementazione dell'algoritmo del simplesso duale per sistemi di equazioni lineari in due variabili. 
Modificando opportunamente le variabili *A*, *b*, *c* e *B*
si potrà leggere lo svolgimento dell'algoritmo passaggio per passaggio.
```python
A = np.array([[1, 0], [0, 1], [1, 2], [-1, 0], [0, -1]]) # matrice dei coefficienti
b = np.array([4, 2, 4, 1, 1]) # matrice dei termini noti
c = np.array([2, 4]) # matrice dei coefficienti della funzione obiettivo
B = np.array([1, 2]) # base di partenza - gli indici partono da 1 anziché da 0
simplesso_duale(A, b, c, B) # chiamata alla funzione
```
