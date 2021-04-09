CC = gcc
CFLAGS += -g -Wall -pedantic -std=c99 -O3
INCLUDES = -I .

TARGETS = Esercizio1/dummyshell \
	Esercizio2/main \
	Esercizio3/main \
	Esercizio4/family \
	Esercizio5/main

.PHONY: all cleanall test
.SUFFIXES: .c .h

%: %.c
	$(CC) $(CFLAGS) $(INCLUDES) $(OPTFLAGS) -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) $(OPTFLAGS) -c -o $@ $<

all : $(TARGETS)

clean :
	-rm -f $(TARGETS)