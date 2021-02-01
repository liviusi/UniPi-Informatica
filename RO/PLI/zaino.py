import numpy as np

def ordina(valore, peso):
	rendimento = np.empty(shape=np.shape(valore))
	for i in range(0, len(valore)):
		rendimento[i] = valore[i] / peso[i]
	indices = np.argsort(rendimento)
	return indices[::-1][:len(indices)] + 1, rendimento

def rilassamentocontinuo(indici, peso, locked, C):
	x = [0] * len(indici)
	c = C
	for i in range(0, len(indici)):
		if locked[indici[i] - 1] == 1: # variabile decisionale forzata a True
			x[indici[i] - 1] = 1
			c -= peso[indici[i] - 1]
		elif locked[indici[i] - 1] == 0: # variabile decisionale forzata a False
			x[indici[i] - 1] = 0
	for i in range(0, len(indici)):
		if c > 0 and (locked[indici[i] - 1] == -1):
			if peso[indici[i] - 1] <= c:
				x[indici[i] - 1] = 1
			else:
				x[indici[i] - 1] = c / float(peso[indici[i] - 1])
			c -= peso[indici[i] - 1]
	return x

def euristico(indici, peso, locked, C):
	x = [None] * len(indici)
	c = C
	for i in range(0, len(indici)):
		if peso[indici[i] - 1] <= c and not (locked[indici[i] - 1] == 0):
			x[indici[i] - 1] = 1
			c -= peso[indici[i] - 1]
		else:
			x[indici[i] - 1] = 0
	return x

def branch(rilassamento, locked):
	lock1 = [0] * len(locked)
	lock2 = [0] * len(locked)
	for i in range(0, len(rilassamento)):
		if not isinstance(rilassamento[i], int):
			lock1[i] = 0
			lock2[i] = 1
		else:
			lock1[i] = locked[i]
			lock2[i] = locked[i]
	return (lock1, lock2)

def calcolavalore(valore, vec):
	ottimo = 0
	for i in range(0, len(vec)):
		ottimo += valore[i] * vec[i]
	return ottimo

def nodi_visitabili(euristica):
	for i in range(0, len(euristica)):
		if euristica[i] != 0:
			return True
	return False

def ammissibile(indici, locked, peso, C):
	c = C
	for i in range(0, len(locked)):
		if (locked[indici[i] - 1] == 1):
			c -= peso[indici[i] - 1]
		if c <= 0:
			return False
	return True


def stampa_variabili_decisionali(locked):
	for i in range(0, len(locked)):
		if locked[i] != -1:
			print("Nodo:", i+1, "->" ,locked[i])

def branch_and_bound(valore, peso, C, locked):
	livello = 0
	result = []
	(indici, rendimento) = ordina(valore, peso)
	print("SETUP")
	print("Rendimento:", rendimento)
	print("Indici:", indici)
	rec_branch_and_bound(result, valore, peso, C, locked, indici, livello)
	ottimo = sorted(result, key=lambda x: x[0])[len(result) - 1]
	print("\n\n\nOttimo:", ottimo[0])
	stampa_variabili_decisionali(ottimo[1])


def rec_branch_and_bound(result, valore, peso, C, locked, indici, livello):
	print("\n\nLIVELLO:", livello)
	stampa_variabili_decisionali(locked)
	if not ammissibile(indici, locked, peso, C):
		print("NESSUNA SOLUZIONE AMMISSIBILE. NODO CHIUSO.")
		return
	ril = rilassamentocontinuo(indici, peso, locked, C)
	x = euristico(indici, peso, locked, C)
	valril = calcolavalore(valore, ril)
	valx = calcolavalore(valore, x)
	print("Rilassamento:", ril, ", valore:", valril)
	print("Euristica:", x, ", valore:", valx)
	result.append( (valx, locked) )
	flag = nodi_visitabili(x)
	if not flag:
		print("\n\nTUTTI I NODI SONO STATI VISITATI.")
	if valx >= valril:
		print("NODO CHIUSO.")
		return
	else:
		(lock1, lock2) = branch(ril, locked) # genera i due nodi dell'albero
		rec_branch_and_bound(result, valore, peso, C, lock1, indici, livello + 1) # visita dfs
		rec_branch_and_bound(result, valore, peso, C, lock2, indici, livello + 1)



def main():
	valore = np.array([11, 23, 18, 6])
	peso = np.array([7, 6, 3, 2])
	C = 8 # capacità
	locked = [-1] * len(valore)
	branch_and_bound(valore, peso, C, locked)

if __name__ == "__main__":
	main()