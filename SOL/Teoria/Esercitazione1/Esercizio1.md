# Punto 1
| --- | DESCRITTORE DI T1i | DESCRITTORE DI T2j | |
| ------- | ----------- | ------------------  | ---- |
| Stato  | Esecuzione | Bloccato | |
| Priorità | 2 | 5 | |
| PC | inv. | inv. | |
| PS | inv. | inv. | |
| SP | inv. | inv. | 1013 (ultima posizione occupata in SP) |
| R1 | inv. | inv. | inv. |
| R2 | inv. | inv. | inv. |


| --- | STACK DEL NUCLEO |
| --- | --- |
| ... | ... |
| 1016 | 23BB |
| 1015 | 2F00 (PC del processore) |
| 1014 | 16F2 (PS del processore) |
| 1013 | 2997 (SP generale) |
| 1012 | |
| 1011 | |


| | PROCESSORE | 
| -- | -- |
| PC | 2900 (vettore delle interruzioni) |
| PS | 275E (parola di stato del nucleo) |
| Stato | Supervisore |

# Punto 2
| --- | DESCRITTORE DI T1i | DESCRITTORE DI T2j | |
| ------- | ----------- | ------------------  | ---- |
| Stato  | Pronto | Esecuzione | |
| Priorità | 2 | 5 | |
| PC | 2F00 | inv. | |
| PS | 16F2 | inv. | |
| SP | 2997 | inv. | 1011 (ultima posizione occupata in SP) |
| R1 | 2649 (R1 generale) | inv. | inv. |
| R2 | 22CE (R2 generale) | inv. | inv. |


| --- | STACK DEL NUCLEO |
| --- | --- |
| ... | ... |
| 1016 | 23BB |
| 1015 | A12C (PC di T2j) |
| 1014 | A6F2 (PS di T2j) |
| 1013 | A275 (SP di T2j) |
| 1012 | A5CC (R1 di T2j) |
| 1011 | A000 (R2 di T2j) |


| --- | PROCESSORE | 
| -- | -- |
| PC | 2900 (vettore delle interruzioni) + k |
| PS | 275E (parola di stato del nucleo) |
| Stato | Supervisore |

# Punto 3
| --- | DESCRITTORE DI T1i | DESCRITTORE DI T2j | |
| ------- | ----------- | ------------------  | ---- |
| Stato  | Pronto | Esecuzione | |
| Priorità | 2 | 5 | |
| PC | inv. | inv. | |
| PS | inv. | inv. | |
| SP | inv. | inv. | A275 (SP di T2j) |
| R1 | inv. | inv. | A5CC (R1 di T2j) |
| R2 | inv. | inv. | A000 (R2 di T2j) |


|  | STACK DEL NUCLEO |
| --- | --- |
| ... | ... |
| 1016 | 23BB |
| 1015 | |
| 1014 | |
| 1013 | |
| 1012 | |
| 1011 | |


| --- | PROCESSORE | 
| -- | -- |
| PC | A12C (PC di T2j) |
| PS | A6F2 (PS di T2j) |
| Stato | Utente |
