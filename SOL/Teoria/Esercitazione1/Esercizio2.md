## Descrittori 
|  | DESCRITTORE DI T1i | DESCRITTORE DI T2j | |
| ------- | ----------- | ------------------  | ---- |
| Stato  | Esecuzione | Bloccato | |
| Priorità | 4 | 3 | |
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

# Punto 1
## Descrittori 
|  | DESCRITTORE DI T1i | DESCRITTORE DI T2j | |
| ------- | ----------- | ------------------  | ---- |
| Stato  | Esecuzione | Bloccato | |
| Priorità | 4 | 3 | |
| PC | inv. | inv. | |
| PS | inv. | inv. | |
| SP | inv. | inv. | 1013 (ultima posizione occupata in SP) |
| R1 | inv. | inv. | inv. |
| R2 | inv. | inv. | inv. |

## Stack del Nucleo
|  | |
| --- | --- |
| ... | ... |
| 1016 | 23BB |
| 1015 | 2F00 (PC del processore) |
| 1014 | 16F2 (PS del processore) |
| 1013 | 2997 (SP generale) |
| 1012 | |
| 1011 | |

## Processore: registri speciali e stato
| | | 
| -- | -- |
| PC | 2900 (vettore delle interruzioni) |
| PS | 275E (parola di stato del nucleo) |
| Stato | Supervisore |

# Punto 2
## Descrittori 
|  | DESCRITTORE DI T1i | DESCRITTORE DI T2j | |
| ------- | ----------- | ------------------  | ---- |
| Stato  | Esecuzione | Pronto | |
| Priorità | 4 | 3 | |
| PC | inv. | inv. | |
| PS | inv. | inv. | |
| SP | inv. | inv. | 1011 (ultima posizione occupata in SP) |
| R1 | inv. | inv. | inv. |
| R2 | inv. | inv. | inv. |

## Stack del Nucleo
|  | |
| --- | --- |
| ... | ... |
| 1016 | inv. |
| 1015 | inv. |
| 1014 | inv. |
| 1013 | inv. |
| 1012 | |
| 1011 | |

## Processore: registri speciali e stato
| | | 
| -- | -- |
| PC | 2900 (vettore delle interruzioni) + k |
| PS | 275E (parola di stato del nucleo) |
| Stato | Supervisore |

# Punto 3
## Descrittori 
|  | DESCRITTORE DI T1i | DESCRITTORE DI T2j | |
| ------- | ----------- | ------------------  | ---- |
| Stato  | Pronto | Esecuzione | |
| Priorità | 2 | 5 | |
| PC | inv. | inv. | |
| PS | inv. | inv. | |
| SP | inv. | inv. | 2997 |
| R1 | inv. | inv. | 2649 |
| R2 | inv. | inv. | 22CE |

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
