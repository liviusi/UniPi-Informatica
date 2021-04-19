CC = gcc
CFLAGS += -g -Wall -pedantic -std=c99 -O3
LFLAGS = -lpthread
INCLUDES = -I .

TARGETS = Esercizio1/main \
	Esercizio2/main

.PHONY: all cleanall test
.SUFFIXES: .c .h

%: %.c
	$(CC) $(CFLAGS) $(INCLUDES) $(OPTFLAGS) -o $@ $< $(LFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) $(OPTFLAGS) -c -o $@ $< $(LFLAGS)

all : $(TARGETS)

clean :
	-rm -f $(TARGETS) *.txt