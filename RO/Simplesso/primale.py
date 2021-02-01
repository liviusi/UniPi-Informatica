import numpy as np

def risolviprimale(A, b, base):
	return np.matmul(np.linalg.inv(np.vstack([A[base[0] - 1, :], A[base[1] - 1, :]])), (np.vstack([b[base[0] - 1], b[base[1] - 1]])))

def risolviduale(A, base, c):
	return np.matmul(c.transpose(),	np.linalg.inv(np.vstack([A[base[0] - 1, :], A[base[1] - 1, :]])))

def ottimo(y):
	for i in y:
		if i <= 0:
			return False
	return True

def minimo_indice(y, base):
	counter = 0
	for i in y:
		if i <= 0:
			return base[counter]
		counter += 1

def trova_W(A, base, minimo):
	x  = 0 if (minimo == base[0]) else 1
	return (- np.linalg.inv(np.vstack([A[base[0] - 1, :], A[base[1] - 1, :]])))[:, x]

def degenere(A, base, W):
	counter = -1
	for i in A:
		counter += 1
		if counter == (base[0] - 1) or counter == (base[1] - 1):
			continue
		if np.matmul(A[counter, :], W) > 0:
			return False
	return True

def trova_indici(A, b, base, x, W):
	lst = []
	counter = -1
	aux = -1
	for i in A:
		counter += 1
		if counter == (base[0] - 1) or counter == (base[1] - 1):
			continue
		print('A(', counter+1, ')Wh =', np.matmul(A[counter, :], W))
		if np.matmul(A[counter, :], W) > 0:
			lst.append(((b[counter] - np.matmul(A[counter, :], x)) / np.matmul(A[counter, :], W)).astype(int).item())
			if aux == -1:
				aux = counter
	if len(lst) == 2:
		return (min(lst), lst.index(min(lst)) + 1)
	else:
		print('Theta ha valore di default -1.')
		return (-1, aux + 1)

def aggiorna_base(base, h, k):
	return np.sort(np.where(base==h, k, base))

def simplesso_primale(A, b, c, base, iterazione=0):
	if iterazione >= 10:
		return False
	iterazione += 1
	end = False
	print('\n\nITERAZIONE:', iterazione)
	print('Base:', base)
	x = risolviprimale(A, b, base)
	y = risolviduale(A, base, c)
	print('Primale:', x)
	print('Duale:', y)
	end = ottimo(y)
	print('Soluzione ottima:', end)
	if end:
		return
	h = minimo_indice(y, base)
	print('h =', h)
	W = trova_W(A, base, h)
	print('W di h:', W)
	end = degenere(A, base, W)
	print('Soluzione degenere:', end)
	if end:
		return
	(theta, k) = trova_indici(A, b, base, x, W)
	print('(theta, k) =', (theta, k))
	B = aggiorna_base(base, h, k)
	simplesso_primale(A, b, c, B, iterazione)


def main():
	A = np.array([[1, 0], [1, 1], [-1, 0], [0, -1]])
	b = np.array([2, 3, 0, 0])
	c = np.array([2, 1])
	B = np.array([3, 4])
	simplesso_primale(A, b, c, B)

	A = np.array([[-2, 1], [1, -1], [-1, 0], [0, -1]])
	b = np.array([1, 1, 0, 0])
	c = np.array([2, -1])
	B = np.array([3, 4])
	simplesso_primale(A, b, c, B)

	A = np.array([[-1, 0], [0, -1], [1, 1], [0, 1]])
	b = np.array([0, 0, 2, 1])
	c = np.array([2, -1])
	B = np.array([1, 4])
	simplesso_primale(A, b, c, B)

if __name__ == "__main__":
	main()