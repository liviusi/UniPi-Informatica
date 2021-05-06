CC = gcc
CFLAGS += -std=c99 -Wall -g
INCLUDES = -I ./src/includes/logging/ -I ./src/includes/parsing/ -I ./src/includes/
LDFLAGS = -L ./src/includes/
OPTFLAGS = -O3
LIBS = -lpthread

.PHONY: all clean cleanall
.SUFFIXES: .c .h

TARGETS = src/server

%: %.c
	$(CC) $(CFLAGS) $(INCLUDES) $(OPTFLAGS) -o $@ $< $(LDFLAGS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) $(OPTFLAGS) -c -o $@ $<

all: $(TARGETS)

src/server: src/server.o src/includes/logging/logging.o src/includes/parsing/parsing.o
	$(CC) $(CFLAGS) $(INCLUDES) $(OPTFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

clean :
	-rm -f $(TARGETS) src/server.o src/includes/logging/*.o src/includes/parsing/*.o