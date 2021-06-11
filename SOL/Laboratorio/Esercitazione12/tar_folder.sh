#!/bin/bash
# shell da usare per l’interpretazione dello script

if [ $# -ne 1 ]; then                        # se non ci sono 1 argomenti
    echo usa: $(basename $0) nomedirectory   # messaggio di errore
    exit -1
fi
dir=$1
if [ ! -d $dir ]; then                       # se l'argomento in input non è una directory
    echo "L'argomento $dir non e' una directory"   
    exit 1;   
fi

bdir=$(basename $dir)
if [ -w $bdir.tar.gz -ne 0 ]; then                     # il file esiste ed e scrivibile
    echo -n "il file $bdir.tar.gz esiste gia', sovrascriverlo (S/N)?"
    read yn                                 # salva in yn l'input dato al comando read
    if [ x$yn != x"S" ]; then               # verifica che l'input sia uguale a "S"
          exit 0;
    fi
    rm -f $bdir.tar.gz
fi
echo "creo l'archivio con nome $bdir.tar.gz"

tar cf $bdir.tar $dir 2> error.txt       # appende l’output sullo std-error nel file error.txt   
if [ $? -ne 0 ]; then                         # controlla che il comando sia andato a buon fine
    echo "Errore nella creazione dell'archivio"
    exit 1
fi
gzip $bdir.tar  2> error.txt                 # appende l’output sullo std-error nel file error.txt
if [ $? -ne 0 ]; then                         # controlla che il comando sia andato a buon fine
    echo
    echo "Errore nella compressione dell'archivio"
    exit 1
fi

echo "archivio creato con successo, il contenuto dell’archivio e':"
tar tzvf $bdir.tar.gz  2>&1           # redirige lo std-error sullo std-output
exit 0