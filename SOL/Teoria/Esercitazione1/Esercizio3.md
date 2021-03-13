# Punto 1
| ---  | DESCRITTORE DI Ti | DESCRITTORE DI Tj |
| --- | --- | --- |
| Stato | Esecuzione | Pronto |
| PC | inv. | inv. |
| SP | inv. | inv. |
| PS | inv. | inv. |
| R1 | inv. | inv. |
| R2 | inv. | inv. |
| R3 | inv. | inv. |
| R4 | inv. | inv. |


| --- | STACK DEL NUCLEO |
| --- | --- |
| ... | ... |
| 1016 | 23BB |
| 1015 | 070A |
| 1014 | 2F00 |
| 1013 | 16F2 |
| 1012 | |
| 1011 | |
| 1010 | |


| REGISTRI | UTENTE | SUPERVISORE |
| --- | --- | --- |
| SP | inv. | 1013 |
| R1 | inv. | inv. |
| R2 | inv. | inv. |
| R3 | inv. | inv. |
| R4 | inv. | inv. |


| --- | PROCESSORE | 
| -- | -- |
| PC | 0425 |
| PS | 275E |


# Punto 2
| ---  | DESCRITTORE DI Ti | DESCRITTORE DI Tj |
| --- | --- | --- |
| Stato | Pronto | Esecuzione |
| PC | 2F00 (PC processore) | inv. |
| SP | 16F2 | inv. |
| PS | 16F2 (PS processore) | inv. |
| R1 | 6649 (R1 stato utente) | inv. |
| R2 | 02CE (R2 stato utente) | inv. |
| R3 | D410 (R3 stato utente) | inv. |
| R4 | 73FF (R4 stato utente) | inv. |


| --- | STACK DEL NUCLEO |
| --- | --- |
| ... | ... |
| 1016 | inv. |
| 1015 | inv. |
| 1014 | A12C |
| 1013 | 16F2 |
| 1012 | |
| 1011 | |
| 1010 | |


| REGISTRI | UTENTE | SUPERVISORE |
| --- | --- | --- |
| SP | A275 (SP di Tj) | 1013 (ultima posizione occupata) |
| R1 | 25CC (R1 di Tj) | ?? |
| R2 | 0000 (R2 di Tj) | ?? |
| R3 | 0056 (R3 di Tj) | ?? |
| R4 | AA38 (R4 di Tj) | ?? |


| --- | PROCESSORE | 
| -- | -- |
| PC | 0425 + ?? |
| PS | inv. |

# Punto 3
| ---  | DESCRITTORE DI Ti | DESCRITTORE DI Tj |
| --- | --- | --- |
| Stato | Pronto | Esecuzione |
| PC | inv. | inv. |
| SP | inv. | inv. |
| PS | inv. | inv. |
| R1 | inv. | inv. |
| R2 | inv. | inv. |
| R3 | inv. | inv. |
| R4 | inv. | inv. |


| --- | STACK DEL NUCLEO |
| --- | --- |
| ... | ... |
| 1016 | inv. |
| 1015 | inv. |
| 1014 | |
| 1013 | |
| 1012 | |
| 1011 | |
| 1010 | |


| REGISTRI | UTENTE | SUPERVISORE |
| --- | --- | --- |
| SP | inv. | 1015 |
| R1 | inv. | inv. |
| R2 | inv. | inv. |
| R3 | inv. | inv. |
| R4 | inv. | inv. |


| --- | PROCESSORE | 
| -- | -- |
| PC | A12C (PC di Tj) |
| PS | 16F2 (PS di Tj) |
