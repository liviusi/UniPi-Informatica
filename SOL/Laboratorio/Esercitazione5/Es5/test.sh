#!/bin/bash

# devo avere almeno un argomento
if [ $# -eq 0 ]; then
    echo "ERRORE: usa: $(basename $0) programma da testare"
    exit -1
fi

# controllo che mi e' stato passato un eseguibile
if [ ! -x $1 ]; then
    echo "$1 non e' eseguibile"
    exit 1
fi

# creo il file filein usando un block-size di 8k e ripetendolo
# per 1500 volte --> size = 12288000 bytes
dd if=/dev/zero of=./filein bs=8192 count=1500 >& /dev/null
if [ $? -ne 0 ]; then
    echo "Errore nella creazione del file filein"
    exit 2
fi

# tolgo il prefisso dal nome del file
filename=$(basename $1)

# rimuovo il vecchio file se c'e'
rm -f ./test-$filename.dat

echo "# $1 test" > ./test-$filename.dat
echo "# size user sys" >> ./test-$filename.dat
for i in 1 4 8 16 32 64 128 256 512 1024 2048 4096; do
    (echo -n "$i " && /usr/bin/time -f "%e %S" $1 ./filein ./fileout $i) >> ./test-$filename.dat 2>&1
done

