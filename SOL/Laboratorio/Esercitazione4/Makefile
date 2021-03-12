CC = gcc
CFLAGS += -Wall -pedantic -g
# TARGETS		= 

.PHONY: clean
.SUFFIXES: .c

all : Esercizio1/main Esercizio2/main Esercizio3/main

Esercizio1/main : Esercizio1/main.c
	$(CC) $(CFLAGS) $^ -o $@

Esercizio2/main : Esercizio2/main.c
	$(CC) $(CFLAGS) $^ -o $@

Esercizio3/main : Esercizio3/main.c
	$(CC) $(CFLAGS) $^ -o $@

clean : 
	-rm -f Esercizio1/main
	-rm -f Esercizio2/main
	-rm -f Esercizio3/main