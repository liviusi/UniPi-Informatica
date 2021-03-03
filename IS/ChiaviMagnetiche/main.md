# Chiavi magnetiche: diagramma dei casi d'uso

## Narrativa

### Assegnare chiavi

|                                          | Descrizione |
| -----------                              | ----------- |
| Nome                                     | Assegnare chiavi.       |
| Breve descrizione                        | Assegnare inizialmente chiavi e diritti corrispondenti.        |
| Attori primari                           | Amministratore. |
| Attori secondari                         | Nessuno. |
| Precondizioni                            | Non esiste una chiave associata al dipendente a cui si fa riferimento. |
| Sequenza degli eventi principale         | <ul><li> Amministratore inserisce i dati del dipendente all'interno della base di dati dei dipendenti. </li> <li> *Verifica*. </li> <li> Il sistema di gestione del personale assegna i diritti al dipendente. </li></ul> |
| Postcondizioni             | Esiste una chiave associata al dipendente a cui si fa riferimento. |
| Sequenza degli eventi alternativa        | *Verifica* fallisce. |

### Modificare chiavi

|                                          | Descrizione |
| -----------                              | ----------- |
| Nome                                     | Modificare chiavi.       |
| Breve descrizione                        | Modifica dei diritti associati al proprietario della chiave.        |
| Attori primari                           | Amministratore. |
| Attori secondari                         | Nessuno. |
| Precondizioni                            | Esiste una chiave associata al dipendente a cui si fa riferimento. |
| Sequenza degli eventi principale         | <ul><li> Amministratore inserisce i dati del dipendente all'interno della base di dati dei dipendenti. </li> <li> *Verifica*. </li> <li> Il sistema di gestione del personale modifica i diritti associati al dipendente. </li></ul> |
| Postcondizioni             | Esiste una chiave associata al dipendente a cui si fa riferimento con i diritti correttamente modificati. |
| Sequenza degli eventi alternativa        | *Verifica* fallisce. |

### Verifica

|                                          | Descrizione |
| -----------                              | ----------- |
| Nome                                     | Verifica.       |
| Breve descrizione                        | Verifica della validità dell'operazione.        |
| Attori primari                           | Nessuno. |
| Attori secondari                         | Sistema di gestione del personale. |
| Precondizioni                            | Nessuna. |
| Sequenza degli eventi principale         | <ul><li> Il sistema verifica l'identità del dipendente. </li> <li> Il sistema verifica i diritti del dipendente. </li></ul> |
| Postcondizioni             | Identità e diritti vengono correttamente verificati. |
| Sequenza degli eventi alternativa        | Almeno una delle verifiche fallisce. |

### Usare chiavi

|                                          | Descrizione |
| -----------                              | ----------- |
| Nome                                     | Usare chiavi.       |
| Breve descrizione                        | Uso della chiave per entrare in una stanza.        |
| Attori primari                           | Dipendenti. |
| Attori secondari                         | Nessuno. |
| Precondizioni                            | Nessuna. |
| Sequenza degli eventi principale         | <ul><li> Il dipendente passa la chiave magnetica. </li> <li> Il sistema di sicurezza verifica che la tessera possa aprire la porta. </li> <li> Il sistema di sicurezza apre la porta. </li></ul> |
| Postcondizioni             | La stanza per la quale si è passata la tessera magnetica ha aperto la propria porta. |
| Sequenza degli eventi alternativa        | La tessera non può aprire la porta, diritti mancanti. |
