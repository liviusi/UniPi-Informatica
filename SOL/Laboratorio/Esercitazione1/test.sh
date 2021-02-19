#!/usr/bin/env bash

if [ $# -ne 1 ]; then
	echo "Deve essere fornito in input il percorso relativo alla cartella con l'esercizio da testare."
	echo "e.g. ./test.sh Esercizio1"
	exit 1
fi

gcc $1/main.c
if [ $? -ne 0 ]; then
	exit 1
fi
./a.out `cat $1/testset/input0.txt` | diff - $1/testset/output0.txt
./a.out `cat $1/testset/input1.txt` | diff - $1/testset/output1.txt
./a.out `cat $1/testset/input2.txt` | diff - $1/testset/output2.txt
./a.out `cat $1/testset/input3.txt` | diff - $1/testset/output3.txt
./a.out `cat $1/testset/input4.txt` | diff - $1/testset/output4.txt
./a.out `cat $1/testset/input5.txt` | diff - $1/testset/output5.txt
rm -f a.out