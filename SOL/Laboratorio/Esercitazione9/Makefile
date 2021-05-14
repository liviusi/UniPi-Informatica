CC = gcc
CFLAGS += -g -Wall -Wextra -Wmissing-prototypes -Wstrict-prototypes -pedantic -std=c99 -O3
LFLAGS = -lpthread
INCLUDES = -I .

TARGETS = Esercizio1/client Esercizio1/server \
	Esercizio2/client Esercizio2/server \
	Esercizio3/client Esercizio3/server

.PHONY: all cleanall test
.SUFFIXES: .c .h

%: %.c
	$(CC) $(CFLAGS) $(INCLUDES) $(OPTFLAGS) -o $@ $< $(LFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) $(OPTFLAGS) -c -o $@ $< $(LFLAGS)

all : $(TARGETS)

clean :
	-rm -f $(TARGETS) Esercizio1/socket Esercizio2/socket Esercizio3/socket