CC = gcc
CFLAGS += -std=c99 -Wall -g
INCLUDES = -I. -I ./utilities/
LDFLAGS = -L. -L ./utilities/
OPTFLAGS = -O3
LIBS = -lpthread
ESERCIZI = Esercizio1 Esercizio2 Esercizio3 Esercizio4

.PHONY: all clean cleanall
.SUFFIXES: .c .h

TARGETS = Esercizio1/main Esercizio2/main Esercizio3/main Esercizio4/main

%: %.c
	$(CC) $(CFLAGS) $(INCLUDES) $(OPTFLAGS) -o $@ $< $(LDFLAGS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) $(OPTFLAGS) -c -o $@ $<

all: $(TARGETS)

Esercizio1/main: Esercizio1/main.o utilities/wrappers.o
	$(CC) $(CFLAGS) $(INCLUDES) $(OPTFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

Esercizio2/main: Esercizio2/main.o utilities/wrappers.o
	$(CC) $(CFLAGS) $(INCLUDES) $(OPTFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

Esercizio3/main: Esercizio3/main.o utilities/wrappers.o utilities/unboundedfifo.o
	$(CC) $(CFLAGS) $(INCLUDES) $(OPTFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

Esercizio4/main: Esercizio4/main.o utilities/wrappers.o utilities/unboundedfifo.o utilities/icl_hash.o
	$(CC) $(CFLAGS) $(INCLUDES) $(OPTFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

clean :
	-rm -f $(TARGETS) utilities/*.o Esercizio1/*.o Esercizio2/*.o Esercizio3/*.o Esercizio4/*.o