# Esercitazione 5

## Esercizio 1
Scrivere un programma, utilizzando chiamate di sistema, che implementi l'equivalente del comando Unix `cp`. Il programma deve accettare 3 argomenti come segue: ` mycp_sc filein fileout [buffersize] `.
L'argomento 'buffersize' e' la dimensione del buffer da utilizzare per le letture e scritture con le SCs `read` e `write` (se non specificato assegnare un valore di default, es. 256bytes): Realizzare quindi lo stesso programma, utilizzando le chiamate di libreria `fread` ed `fwrite`. Chiamiamo questa seconda versione 'mycp_std'. Confrontare le prestazioni (usando il comando time da linea di comando) del programma 'mycp_sc' e 'mycp_std' al variare del parametro 'buffersize' (provare un pò di casi per buffersize: 1 4 8 16 32 ….. 8192). Che cosa si nota?

## Esercizio 2
Realizzare il comando myfind ispirato al comando di shell `find` `myfind dir nomefile`
che cerca ricorsivamente il file `nomefile` nel sottoalbero radicato nella directory `dir`. Per ogni file `nomefile` trovato, stampa il path assoluto della directory in cui e' stato trovato e la data dell'ultima modifica.

## Esercizio 3
Scrivere un programma C che dati come argomenti una lista di file o directories stampa per ogni argomento: il nome del file/directory, il numero dell'inodo associato, il tipo di file (-,l,d,s,…) i bit di protezione (es. rw-r–r–), l'user identifier (uid), il group identifier (gid), la size del file ed il timestamp dell'ultima modifica. Per convertire il tempo di ultima modifica in un formato stampabile usare la funzione di libreria ctime. Usare getpwuid e getgrgid per convertire uid e gid nei nomi corrispondenti agli id.

## Esercizio 4
Scrivere un programma C chiamato 'lsdir' che, dato come argomento un nome di directory, visita ricorsivamente tutto il sottoalbero di directory che ha come radice la directory passata come argomento. Per ogni directory, il programma deve stampare sullo standard output le informazioni sui file nel seguente formato:
```
Directory: <nomedir1>
file1     size    permessi
file2     size    permessi
------------------
Directory: <nomedir2>
file3     size    permessi
file4     size    permessi
------------------
```
dove la directory 'nomedir1' contiene i file 'file1' e 'file2' e la directory 'nomedir2'. Durante la visita dell'albero di directory non cosiderare le directories '.' (punto) e '..' (punto-punto).
