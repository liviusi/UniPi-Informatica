CC = gcc
AR = AR
CFLAGS += -Wall -pedantic -g -std=c99 -O0
INCLUDES = -I .
TARGETS = Esercizio1/mycp_sc \
	Esercizio1/mycp_std \
	Esercizio2/myfind \
	Esercizio3/permessi \
	Esercizio4/lsdir


.PHONY: all cleanall test
.SUFFIXES: .c .h

%: %.c
	$(CC) $(CFLAGS) $(INCLUDES) $(OPTFLAGS) -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) $(OPTFLAGS) -c -o $@ $<

all : $(TARGETS)

test : Esercizio1/mycp_sc Esercizio1/mycp_std
	Esercizio1/test.sh Esercizio1/mycp_sc
	Esercizio1/test.sh Esercizio1/mycp_std
	gnuplot Esercizio1/plot.gnp

cleanall : 
	-rm -f $(TARGETS) *.dat plot.eps plot.pdf