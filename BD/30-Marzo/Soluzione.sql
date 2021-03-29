-- ESERCIZIO 1

SELECT DISTINCT
	Docenti.Dipartimento, count(*)
FROM
	Docenti
GROUP BY
	Docenti.IdD
HAVING
	count(*) > 30


-- ESERCIZIO 2

SELECT DISTINCT 
	DocentiCorsi.Dipartimento, count(DocentiCorsi.IdD), count(DocentiCorsi.NumeroCorsi), sum(DocentiCorsi.NumStudenti)
FROM
	Docenti JOIN Corsi USING (IdD) as DocentiCorsi
GROUP BY
	DocentiCorsi.Dipartimento


-- ESERCIZIO 3

SELECT DISTINCT
	ACAC.IdA, ACAC.Nome, ACAC.Capienza, ACAC.Giorno, ACAC.NumStudenti
FROM
	(Aule JOIN AuleCorsi USING (IdA) as ACA)
	JOIN
	Corsi USING (IdC) as ACAC
WHERE ACAC.NumStudenti > ACAC.Capienza


-- ESERCIZIO 4
SELECT DISTINCT
	ACA.Nome, ACA.Capienza, ACA.IdA, count(DISTINCT ACA.IdC)
FROM
	Aule JOIN AuleCorsi USING (IdA) as ACA


-- ESERCIZIO 5
SELECT DISTINCT
	D1.Nome, D1.Cognome, D1.Dipartimento, D2.Nome, D2.Cognome, D2.Dipartimento
FROM
	Docenti D1, Docenti D2
WHERE
	D1.Nome = D2.Nome AND D1.Cognome = D2.Cognome AND D1.IdD <> D2.IdD


-- ESERCIZIO 6
SELECT DISTINCT
	DC.IdC, DC.NomeC, DC.NumStudenti
FROM
	Docenti JOIN Corsi USING (IdC) as DC
WHERE
	DC.Dipartimento = 'Informatica'


-- ESERCIZIO 7
SELECT DISTINCT
	DC.Nome, DC.IdD, DC.NumStudenti
FROM
	Docenti JOIN Corsi USING (IdD) as DC
WHERE
	100 < DC.NumStudenti


-- ESERCIZIO 8
SELECT DISTINCT
	Docenti.Nome, Docenti.IdD
FROM
	Docenti
WHERE
	Docenti.IdC not IN (
		SELECT DISTINCT
			DC.Nome, DC.IdD, DC.NumStudenti
		FROM
			Docenti JOIN Corsi USING (IdD) as DC
		WHERE
			DC.NumStudenti > 100
)

-- ESERCIZIO 9
SELECT DISTINCT
	Docenti.Nome, Docenti.IdD
FROM
	Docenti
WHERE
	Docenti.IdC not IN (
		SELECT DISTINCT
			DC.Nome, DC.IdD, DC.NumStudenti
		FROM
			Docenti JOIN Corsi USING (IdD) as DC
		WHERE
			DC.NumStudenti <= 100
)