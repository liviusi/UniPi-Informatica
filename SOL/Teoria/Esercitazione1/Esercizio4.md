# Punto 1
| --- | DESCRITTORE DI P |
| --- | --- |
| Stato | Esecuzione |
| PC | inv. |
| PS | inv. |
| SP | inv. |
| R1 | inv. |
| R2 | inv. |

| --- | SIGNAL STACK DI P |
| --- | --- |
| 2A19 | 5F80 (PC di P) |
| 2A18 | FA00 (SP di P) |
| 2A17 | 56A9 (R1 di P) |
| 2A16 | 52BE (R2 di P) |
| 2A15 |  |
| 2A14 |  |

| --- | REGISTRI |
| --- | --- |
| PC | 3100 |
| PS | 16F2 |
| STATO | Utente |
| SP | 2A15 (prima posizione libera) |
| R1 | ?? |
| R2 | ?? |

# Punto 2
| --- | DESCRITTORE DI P |
| --- | --- |
| Stato | Esecuzione |
| PC | inv. |
| PS | inv. |
| SP | inv. |
| R1 | inv. |
| R2 | inv. |

| --- | SIGNAL STACK DI P |
| --- | --- |
| 2A19 | 5F80 |
| 2A18 | FA00 |
| 2A17 |  |
| 2A16 |  |
| 2A15 |  |
| 2A14 |  |

| --- | REGISTRI |
| --- | --- |
| PC | 3100 + k |
| PS | 16F2 |
| STATO | Utente |
| SP | 2A17 (Prima posizione libera) |
| R1 | 56A9 (Ripristinato dal signal stack) |
| R2 | 52BE (Ripristinato dal signal stack) |

# Punto 3
| --- | DESCRITTORE DI P |
| --- | --- |
| Stato | Esecuzione |
| PC | inv. |
| PS | inv. |
| SP | inv. |
| R1 | inv. |
| R2 | inv. |

| --- | SIGNAL STACK DI P |
| --- | --- |
| 2A19 | |
| 2A18 | |
| 2A17 |  |
| 2A16 |  |
| 2A15 |  |
| 2A14 |  |

| --- | REGISTRI |
| --- | --- |
| PC | 5F80 |
| PS | 16F2 |
| STATO | Utente |
| SP | FA00 |
| R1 | 56A9 |
| R2 | 52BE |
