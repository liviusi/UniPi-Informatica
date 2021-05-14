# Esercitazione 10
## Esercizio 1
Scrivere un programma C che conta il numero di volte in cui l'utente invia il segnale SIGINT (Ctl-C) al processo. Quando il processo riceve il segnale SIGTSTP (Ctl-Z), il numero di SIGINT ricevuti viene stampato sullo standard output. Al terzo segnale SIGTSTP, il processo chiede all'utente se deve terminare o no attendendo una risposta per 10 secondi dallo standard input. Se l'utente non risponde entro 10 secondi un segnale SIGALRM termina il processo.

## Esercizio 2
Modificare l'Esercizio 2 dell'Esercitazione 9 in modo da gestire la terminazione del server a seguito della ricezione di un segnale di terminazione (SIGINT, SIGQUIT, SIGTERM, SIGHUP) con l'obiettivo di lasciare il sistema in uno stato consistente, cioè eliminare dal file system il socket AF_UNIX creato per accettare le connessioni, eventuali file temporanei e liberare la memoria allocata dinamicamente al fine di poter controllare eventuali memory leaks con valgrind. Il segnale SIGPIPE deve essere invece ignorato. La gestione dei segnali deve essere effettuata installando un signal-handler con la SC sigaction. Il signal-handler deve accedere solamente a variabili dichiarate volatile sig_atomic_t e deve usare solo chiamate asynchronous-signal-safe (vedere anche man 7 signal-safety).
Testare l'esecuzione del server lanciandolo con il comando valgrind –leak-check=full e verficare che non ci siano messaggi di errore all'uscita dopo aver inviato un segnale di terminazione al termine del test.

## Esercizio 3
Modificare l'Esercizio 2 in modo da gestire i segnali nel server in modo sincrono utilizzando un thread dedicato (non devono essere installati signal-handlers – se non per ignorare SIGPIPE) e la chiamata di libreria sigwait. Fare attenzione a bloccare tutti i segnali che si vogliono gestire in tutti i threads al fine di evitare il delivery “accidentale” ad un thread diverso dal thread gestore dei segnali.
