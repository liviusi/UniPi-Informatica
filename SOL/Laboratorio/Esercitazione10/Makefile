CC = gcc
CFLAGS += -g -Wall -Wextra -Wmissing-prototypes -pedantic -std=c99 -O3
INCLUDES = -I .
LFLAGS = -lpthread

TARGETS = Esercizio1/main Esercizio2/client Esercizio2/server Esercizio3/client Esercizio3/server

.PHONY: all cleanall
.SUFFIXES: .c .h

%: %.c
	$(CC) $(CFLAGS) $(INCLUDES) $(OPTFLAGS) -o $@ $< $(LFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) $(OPTFLAGS) -c -o $@ $< $(LFLAGS)

all : $(TARGETS)

clean :
	-rm -f $(TARGETS) Esercizio2/socket Esercizio3/socket