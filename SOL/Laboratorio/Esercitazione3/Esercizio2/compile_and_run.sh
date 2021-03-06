#!/usr/bin/env bash


gcc -fPIC -I ./includes/ -Wall -g src/tokenizer_lib.c -c -o objs/tokenizer.o
gcc -shared -o mylibs/libutils.so objs/tokenizer.o
gcc -I ./includes/ -Wall -g tokenizer_main.c -o ./bin/static -L ./mylibs/ -lutils


LD_LIBRARY_PATH="mylibs/" ./bin/static "lorem ipsum dolor sit amet"
rm mylibs/*
rm objs/*
rm bin/*