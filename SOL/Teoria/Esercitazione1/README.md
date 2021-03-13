# ESERCIZIO 1: Cambio Di Contesto - System Call.
Si consideri un processore che dispone dei registri speciali **PC** (program counter) e **PS** (program
status), dello stack pointer **SP** e dei registri generali **R1** e **R2**. Il sistema riserva in memoria un’area
per il **vettore di interruzione** e per lo **stack del nucleo**.
Al riconoscimento di un’interruzione, l’hardware copia nei registri PS e PC il vettore
dell’interruzione (instaurando lo stato **supervisore**, disabilitando le interruzioni e saltando al punto
di ingresso della funzione di servizio) e nel registro SP il valore attuale dello stack pointer del
nucleo.
Ogni funzione di servizio termina con l’istruzione **IRET**, che ripristina tutti i registri dallo stack del
nucleo.
I processi cositituiscono l’unica unità di schedulazione e la politica di gestione del processore è
basata unicamente sulla **priorità dei processi**: quando il sistema è in stato utente, deve essere in
esecuzione il processo non sospeso di priorità massima.
Al tempo **t** sono presenti, tra gli altri, il processo **T1i**, in stato di esecuzione, e il processo **T2j**, che è
bloccato. Al tempo t il processo T1i invoca una funzione che esegue l’istruzione **SVC** (system call),
attivando una funzione di servizio che determina la riattivazione del processo T2j. Al momento
dell’esecuzione dell’istruzione SVC i registri del processore, i descrittori di T1i e T2j e lo stack del
nucleo hanno i contenuti mostrati in tabella. Lo stack pointer del nucleo ha il valore **1016**.
Il vettore di interruzione associato all’interruzione generata da SVC è **2900** e la parola di stato del
nucleo è **275E**.
Si chiede:
- il contenuto dei descrittori, dei registri generali e speciali, dello stack del nucleo e lo stato
del processore durante la fase di estrazione della prima istruzione della funzione di servizio;
- il contenuto dei descrittori, dei registri generali e speciali, dello stack del nucleo e lo stato
del processore durante la fase di estrazione dell’istruzione IRET con la quale termina la
chiamata di sistema;
- il contenuto dei descrittori, dei registri generali e speciali, dello stack del nucleo e lo stato
del processore durante la fase di estrazione dell’istruzione eseguita subito dopo la IRET.

## Descrittori 
|  | DESCRITTORE DI T1i | DESCRITTORE DI T2j | |
| ------- | ----------- | ------------------  | ---- |
| Stato  | Esecuzione | Bloccato | |
| Priorità | 2 | 5 | |
| PC | 2E31 | A12C | |
| PS | 26F2 | A6F2 | |
| SP | 2873 | A275 | 2997 |
| R1 | 2234 | A5CC | 2649 |
| R2 | 26CC | A000 | 22CE |

## Stack del Nucleo
|  | |
| --- | --- |
| ... | ... |
| 1016 | 23BB |
| 1015 | |
| 1014 | |
| 1013 | |
| 1012 | |
| 1011 | |

## Processore: registri speciali e stato
| | | 
| -- | -- |
| PC | 2F00 |
| PS | 16F2 |
| Stato | Utente |

# ESERCIZIO 2: Cambio Di Contesto - System Call.
Risolvere l'esercizio precedente nell'ipotesi che la priorità di T1i sia **4** e quella di T2j sia **3**.

# Esercizio 3: Cambio Di Contesto - Timer.
Si consideri un processore che dispone dei seguenti registri:
- i registri speciali **PC** (program counter) e **PS** (program status);
- un registro stack pointer **SP**;
- un banco di registri generali **R1**, **R2**, **R3**, **R4**;
- un banco di registri generali **R’1**, **R’2**, **R’3**, **R’4** e **SP’** utilizzati quando il processore commuta in
stato supervisore.

Nel sistema sono presenti, tra gli altri, il processo **Ti**, in stato di **esecuzione**, e il processo **Tj**, che
occupa la **prima posizione** della Coda **Pronti**. Al tempo **t** viene generata un’**interruzione dal timer**
che segnala l’esaurimento del “quanto di tempo” a disposizione del processo Ti. In base alla politica
di scheduling utilizzata, deve essere messo in esecuzione il processo Tj. Immediatamente dopo il
tempo t, quando l’interruzione viene riconosciuta, i registri del processore, i descrittori e gli stack di
Ti e Tj e del nucleo hanno i contenuti mostrati in figura.
Supponendo che il **vettore di interruzione** associato all’interruzione **TimerInt** sia **0425** e che la
parola di stato del nucleo sia **275E**, si chiede:
- il contenuto dei descrittori, dei registri generali e speciali e dello stack del nucleo durante la
fase di estrazione della prima istruzione della funzione di servizio;
- il contenuto dei descrittori, dei registri generali e speciali e dello stack del nucleo durante la
fase di estrazione dell’istruzione IRET con la quale termina la funzione di servizio;
- il contenuto dei descrittori, dei registri generali e speciali e dello stack del nucleo durante la
fase di estrazione dell’istruzione eseguita subito dopo la IRET.
## Descrittori
| DESCRITTORE DI Ti | DESCRITTORE DI TJ |
| --- | --- |
| Stato | Esecuzione | Pronto |
| PC | 2E31 | A12C |
| SP | 2873 | A275 |
| PS | 16F2 | 16F2 |
| R1 | 1234 | 25CC |
| R2 | 56CC | 0000 |
| R3 | 0000 | 0056 |
| R4 | 0000 | AA38 |
## Stack del Nucleo
| | |
| --- | --- |
| ... | ... |
| 1016 | 23BB |
| 1015 | 070A |
| 1014 | |
| 1013 | |
| 1012 | |
| 1011 | |
| 1010 | |
## Registri
| | UTENTE | SUPERVISORE |
| --- | --- | --- |
| SP | 2997 | 1015 |
| R1 | 6649 | 0012 |
| R2 | 02CE | AACC |
| R3 | D410 | 2345 |
| R4 | 73FF | 789A |
## Processore: registri speciali
| | | 
| -- | -- |
| PC | 2F00 |
| PS | 16F2 |


# Esercizio 4: Cambio Di Contesto - Upcall.
Si consideri un processore che dispone dei registri speciali **PC** (program counter) e **PS** (program
status), dello stack pointer **SP** e dei registri generali **R1** e **R2**. In stato utente, ogni processo dispone
di uno **stack ad uso generale** e di uno **stack riservato** per gestire le upcall chiamato **Signal Stack**.
Per notificare una upcall ad un processo, il nucleo salva il program counter, lo stack pointer del
processo e tutti i registri generali nel Signal Stack, quindi modifica lo stack pointer per puntare al
Signal Stack, e il program counter per puntare alla procedura di gestione della upcall. La upcall si
conclude ripristinando i registri generali dal Signal Stack, e quindi con l’istruzione **RETU** che
ripristina PC e SP sempre dal Signal Stack.
Al tempo **t**, il nucleo riceve la richiesta di notifica di una upcall dal processo **P** che corrisponde alla
funzione di gestione che inizia all’indirizzo **3100** (nello spazio di memoria del processo). In questo
istante di tempo, il signal stack punta alla locazione **2A19**, il processo è in stato di **pronto** e il
contenuto dei suoi registri speciali e generali è conservato nel suo descrittore come mostrato in
tabella.
| | DESCRITTORE DI P |
| --- | --- |
| Stato | Esecuzione |
| PC | 5F80 |
| PS | 16F2 |
| SP | FA00 |
| R1 | 56A9 |
| R2 | 52BE |

Mostrare:
- Il contenuto dei descrittori, dei registri generali e speciali, dello stack del nucleo e lo stato del
processore durante la fase di estrazione della prima istruzione della funzione di gestione della
upcall;
- Il contenuto dei descrittori, dei registri generali e speciali, dello stack del nucleo e lo stato del
processore durante la fase di estrazione dell’istruzione RETU con la quale termina la upcall;
- Il contenuto dei descrittori, dei registri generali e speciali, dello stack del nucleo e lo stato del
processore durante la fase di estrazione dell’istruzione eseguita subito dopo la RETU. 
