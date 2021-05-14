# Esercitazione 9

## Esercizio 1
Realizzare in C un programma client ed un programma server. I due processi intergiscono utilizzando socket AF_UNIX. Il client apre una connessione verso il server ed invia richieste (sotto forma di stringhe) corrispondenti ad operazioni aritmetiche inserite dall'utente (es. 2+1, 3/4 + 2/7, sqrt(5) + 3.14, …). Il server gestisce una connessione alla volta verso un client. Il client, dopo l'invio della stringa contenente l'operazione attende il risultato dal server prima di inviare una nuova richiesta. Per eseguire l'operazione, il server lancia un processo che esegue la calcolatrice testuale 'bc'. Il messaggio di risposta del server è una stringa contenente il risultato calcolato. La sequenza di operazioni termina quando il client riceve in input la stringa “quit” dall'utente che fa chiudere la connessione con il server. Il server si rimette in attesa di ricevere una nuova connessione.

## Esercizio 2
Realizzare un programma C che implementa un server che rimane sempre attivo in attesa di richieste da parte di uno o piu' processi client su una socket di tipo AF_UNIX. Ogni client richiede al server la trasformazione di tutti i caratteri di una stringa da minuscoli a maiuscoli (es. ciao –> CIAO). Per ogni nuova connessione il server lancia un thread POSIX che gestisce tutte le richieste del client (modello “un thread per connessione”) e quindi termina la sua esecuzione quando il client chiude la connessione.
Per testare il programma, lanciare piu' processi client ognuno dei quali invia una o piu' richieste al server multithreaded.

## Esercizio 3
Realizzare il server dell'Esercizio 2 secondo il modello single-threaded. Il server fa uso della chiamata di sistema select (o poll) per gestire sia nuove connessioni da parte di nuovi client che la gestione dei messaggi inviati dai client gia' connessi.