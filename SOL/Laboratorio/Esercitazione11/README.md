# Esercitazione 11
## Esercizio 1
Realizzare l'Esercizio 2 dell'Esercitazione 10 con un pool di N thread (N è un parametro del programma) secondo il modello Manager-Workers dove però il generico thread Worker gestisce interamente tutta le richieste di un client connesso. Gestire i segnali SIGINT e SIGQUIT per la terminazione consistente del server.

## Esercizio 2
Realizzare una seconda versione dell'Esercizio 1 (sempre secondo lo schema Manager-Workers con thread pool) in cui il generico thread Worker gestisce solamente una richiesta di uno dei client connessi.