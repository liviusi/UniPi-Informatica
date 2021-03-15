# Basi di Dati - Esercizio 16 Marzo

Si consideri il seguente schema relazionale, che descrive Aule, Corsi, e l’orario
delle lezioni dei corsi nelle aule. L’orario è settimanale, per cui l’attributo Giorno
ha solo sette valori possibili.
Molte interrogazioni fanno riferimento al “numero di studenti”: si tratta
dell’attributo NumStudenti della tabella Corsi.

`Aule(IdA, NomeAula, Edificio, Capienza)`.

`AuleCorsi(IdA*, IdC*, Ora, Giorno)`.

`Corsi(IdC, NomeCorso, AnnoAccademico, NumStudenti, Docente)`.

Per ciascuna interrogazione, si scriva l’epressione algebrica che la risolve.

## Esercizio 1
Per ogni lezione in cui il giorno è ‘Giovedì’ si indichi Nome aula, ora,
Nome-corso.

## Esercizio 2
Per ogni corso che ha almeno una lezione il giovedì, si indichi NomeCorso e
Docente.

## Esercizio 3
Per ogni corso che ha solo lezione il giovedì, si indichi NomeCorso e
Docente.

## Esercizio 4
Per ogni giorno della settimana e per ogni aula (ovvero, per ogni coppia
giorno-aula) si calcoli il numero di corsi che hanno luogo quel giorno in
quell’aula, e il numero totale di studenti che transitano per l’aula.

## Esercizio 5
Per ogni corso dell’AnnoAccademico 2020 che ha almeno tre lezioni
(ovvero, tre righe nella tabella AuleCorsi) riportare NomeCorso.

## Esercizio 6
Per ogni coppia Docente-AnnoAccademico, riportare il numero di corsi
insegnati dal docente in quell’anno e il numero totale degli studenti.

## Esercizio 7
Non facile: per ogni coppia di corsi C1-C2 dello stesso anno accademico che
hanno almeno una righa nella tabella AuleCorsi con la stessa aula e lo stesso
Giorno, riportare la coppia C1.NomeCorso – C2.NomeCorso.
