#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include <util.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
	fprintf(stderr, "usa: %s infile outfile [bufsize]\n", argv[0]);
	return EXIT_FAILURE;
    }
    char *infile  = argv[1];
    char *outfile = argv[2];
    size_t bufsize = BUFSIZE;
    if (argc >=4) {
      long sz;
      if (isNumber(argv[3], &sz) != 0 || sz<=0) {
	fprintf(stderr, "l'argomento %s non e' un numero valido\n", argv[3]);
	return EXIT_FAILURE;
      }
      bufsize=sz;
    }
    
    int fin;
    SYSCALL_EXIT(open, fin, open(infile, O_RDONLY),"Aprendo il file in input %s : errno=%d\n", infile, errno);	

    // imposto la maschera che voglio per la creazione dei file
    mode_t oldmask = umask(033);
    int fout;
    // creo il file con diritti 0644
    SYSCALL_EXIT(open,fout,open(outfile, O_CREAT|O_WRONLY|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH),
	    "Creando il file in output %s : errno=%d\n", infile, errno);	
    // ripristino la vecchia maschera
    umask(oldmask);
    char *buf = malloc(bufsize*sizeof(char));
    if (!buf) { perror("malloc"); return errno;} 
    size_t len;
    do {
	SYSCALL_EXIT(read,len,read(fin,buf,bufsize),"Leggendo il file in input %s:errno=%d\n",infile,errno);
	if (len) 
	  SYSCALL_EXIT(write,len,write(fout,buf,len),"Scrivendo il file in output %s : errno=%d\n", outfile, errno);
    } while(len>0);
    SYSCALL_EXIT(close,len,close(fin), "Chiudendo il file in input %s : errno=%d\n", infile, errno);
    // scommentando la chiamata successiva faccio il flush dei buffer in kernel space
    //SYSCALL_EXIT(fsync,len,fsync(fout),"Facendo il flush del file in output %s : errno=%d\n", outfile, errno);
    SYSCALL_EXIT(close,len,close(fout), "Chiudendo il file in output %s : errno=%d\n", outfile, errno);
    free(buf);
    return 0;
}
