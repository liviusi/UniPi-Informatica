#!/usr/bin/env bash

head -c 1KB /dev/urandom | tr -dc 'a-zA-Z0-9~!@#$%^&*_-' | fold -w 50 > fileToSend.txt
javac Esercizio.java
java Esercizio 1010
../cleanup.sh .