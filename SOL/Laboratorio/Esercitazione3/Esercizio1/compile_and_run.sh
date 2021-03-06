#!/usr/bin/env bash

gcc -I ./includes/ -Wall -g src/tokenizer_lib.c -c -o objs/tokenizer.o
ar rvf mylibs/libutils.a objs/tokenizer.o
gcc -I ./includes/ -Wall -g tokenizer_main.c -o ./bin/static -L ./mylibs/ -lutils


./bin/static "lorem ipsum dolor sit amet"

rm mylibs/*
rm objs/*
rm bin/*