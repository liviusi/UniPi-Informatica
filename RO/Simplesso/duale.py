import numpy as np

def risolviduale(A, base, c):
	return np.matmul(c.transpose(),	np.linalg.inv(np.vstack([A[base[0] - 1, :], A[base[1] - 1, :]])))

def ammissibile(y):
	for i in y:
		if i >= 0:
			return True
	return False

def risolviprimale(A, b, base):
	return np.matmul(np.linalg.inv(np.vstack([A[base[0] - 1, :], A[base[1] - 1, :]])), (np.vstack([b[base[0] - 1], b[base[1] - 1]])))

def ottimo(A, b, base, x):
	counter = -1
	for i in A:
		counter += 1
		if counter == (base[0] - 1) or counter == (base[1] - 1):
			continue
		if (np.matmul(A[counter, :], x).astype(int).item()) > b[counter]:
			return False
	return True

def trova_k(A, b, base, x):
	lst = []
	counter = -1
	flag = True
	for i in A:
		counter += 1
		if counter == (base[0] - 1) or counter == (base[1] - 1):
			continue
		print('A(', counter+1, ')x =', (np.matmul(A[counter, :], x).astype(int).item()))
		if np.matmul(A[counter, :], x) > b[counter]:
			lst.append(((np.matmul(A[counter, :], x).astype(int).item()), counter + 1))
		if np.matmul(A[counter, :], x) > 0:
			flag = False
	print(lst)
	if (not(lst)):
		if flag: # se tutti i prodotti sono negativi
			return -1 # allora è degenere
		else:
			return -2 # non è degenere
	return min(lst, key = lambda t: t[1])[1]

def trova_eta(A, k, base):
	return np.matmul(A[k - 1, :], np.linalg.inv(np.vstack([A[base[0] - 1, :], A[base[1] - 1, :]])))

def degenere(eta):
	for i in eta:
		if i >= 0:
			return False
	return True

def trova_h(A, base, y, eta):
	lst = []
	counter = -1
	aux = -1
	for i in A:
		counter += 1
		if counter != (base[0] - 1) and counter != (base[1] - 1):
			continue
		else:
			aux += 1
			if eta[aux] <= 0:
				continue
			else:
				lst.append(((y[aux] / eta[aux]).astype(int).item(), counter + 1))
	if len(lst) != 2:
		return min(lst, key = lambda t: t[0])[1]
	else:
		print('theta =', min(lst, key = lambda t: t[0])[0])
		return (min(lst, key = lambda t: t[0])[1])

def aggiorna_base(base, h, k):
	return np.sort(np.where(base==h, k, base))

def simplesso_duale(A, b, c, base, iterazione=0):
	if iterazione >= 4:
		return False
	iterazione += 1
	flag = False
	print('\n\nITERAZIONE:', iterazione)
	print('Base:', base)
	y = risolviduale(A, base, c)
	x = risolviprimale(A, b, base)
	d = []
	aux = 0
	for i in range(0, len(A)):
		if i == base[aux]:
			d.append(y[aux].astype(int).item())
		else:
			d.append(0)
	print('Duale:', d)
	print('Primale:', x)
	flag = ammissibile(y)
	print('Soluzione ammissibile:', flag)
	if not flag:
		return
	flag = ottimo(A, b, base, x)
	print('Soluzione ottima:', flag)
	if flag:
		aux = trova_k(A, b, base, x)
		if aux == -1:
			print("Soluzione degenere: True")
		else:
			print("Soluzione degenere: False")
		return
	k = trova_k(A, b, base, x)
	print('k =', k)
	eta = trova_eta(A, k, base)
	print('eta =', eta)
	flag = degenere(eta)
	print('Soluzione degenere:', flag)
	if flag:
		return
	h = trova_h(A, base, y, eta)
	print('h =', h)
	simplesso_duale(A, b, c, aggiorna_base(base, h, k), iterazione)


def main():

	A = np.array([[1, 0], [0, 1], [1, 2], [-1, 0], [0, -1]])
	b = np.array([4, 2, 4, 1, 1])
	c = np.array([2, 4])
	B = np.array([1, 2])
	simplesso_duale(A, b, c, B)

	A = np.array([[0, 1], [-1, 2], [-1, 0], [-2, 1], [-1, 0]])
	b = np.array([4, 10, 1, 4, 0])
	c = np.array([0, 1])
	B = np.array([1, 2])
	simplesso_duale(A, b, c, B)

if __name__ == "__main__":
	main()