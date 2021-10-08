#!/usr/bin/env bash

rm Appunti.pdf 2> /dev/null
cd src
GLOBIGNORE=*.tex:*.pdf
rm -v * 2> /dev/null
cd ..
mv src/Appunti.pdf . 2> /dev/null