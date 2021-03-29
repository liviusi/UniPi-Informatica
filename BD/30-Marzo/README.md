# Basi di Dati - Esercizio 30 Marzo 

Si consideri il seguente schema relazionale. Molte interrogazioni fanno riferimento al
“numero di studenti”: si tratta dell’attributo NumStudenti della tabella Corsi.

`Aule(IdA, Nome, Edificio, Capienza)`.

`AuleCorsi(IdA*, IdC*, Ora, Giorno)`.

`Corsi(IdC, NomeC, AnnoAccademico, NumStudenti, IdD*)`.

`Docenti(IdD, Nome, Cognome, Dipartimento)`.

Scrivere le interrogazioni SQL che restituiscono, senza ripetizioni, le seguenti informazioni:
## Esercizio 1
Per ogni Dipartimento con più di 30 docenti, riportare il nome del Dipartimento e il numero di
docenti che vi appartengono.
## Esercizio 2
Per ogni Dipartimento, riportare il nome del Dipartimento, il numero di docenti che vi
appartengono, il numero di Corsi che vengono insegnati dai docenti di quel dipartimento e il
numero totali di studenti di tali corsi.
## Esercizio 3
Per ogni aula in cui si svolge un qualche corso con un numero di studenti maggiore della
capienza dell’aula, riportare IdA, Nome, Capienza, Giorno del corso, e numero di studenti.
## Esercizio 4
Per ogni aula riportare il nome, la capienza, l’IdA, e il numero di corsi diversi che si tengono
in quell’aula.
## Esercizio 5
Per ogni coppia di docenti con lo stesso cognome, riportare il nome e il cognome e il
dipartimento del primo e il nome e il cognome e il dipartimento del secondo.
## Esercizio 6
Per ogni corso in cui il docente appartiene al Dipartimento di ‘Informatica’ riportare IdC,
Nome C, NumStudenti.
## Esercizio 7
Per ogni docente che tiene almeno un corso con più di 100 studenti riportare il nome del
docente, il suo IdD, e il numero di studenti.
## Esercizio 8
Per ogni docente che non tiene nessun corso con più di 100 studenti riportare il nome del
docente e il suo IdD.
## Esercizio 9
Per ogni docente che tiene solo corsi con più di 100 studenti riportare il nome del docente e il
suo IdD.
