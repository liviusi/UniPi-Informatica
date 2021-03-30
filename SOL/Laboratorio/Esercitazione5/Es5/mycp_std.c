#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include <util.h>

// prototipo
int fileno(FILE*);

int main(int argc, char *argv[]) {
    if (argc < 3) {
	fprintf(stderr, "use: %s infile outfile [bufsize]\n", argv[0]);
	return EXIT_FAILURE;
    }
    char *infile  = argv[1];
    char *outfile = argv[2];
    size_t bufsize = BUFSIZE;
    if (argc >=4) bufsize = atol(argv[3]);
    
    FILE *fin;

    if ((fin = fopen(infile, "r")) == NULL) {
	perror("ERROR: open");
	int err = errno;
	print_error("Aprendo il file in input %s : errno=%d\n", infile, err);	
	return err;
    }
    // imposto la maschera che voglio
    mode_t oldmask = umask(033);
    FILE *fout;
    // creo il file con diritti 0644
    if ((fout = fopen(outfile, "w+")) == NULL) { 
	perror("ERROR: open");
	int err = errno;
	print_error("Creando il file in output %s : errno=%d\n", infile, errno);	
	return err;
    }
    // ripristino la vecchia maschera
    umask(oldmask);
    char *buf = malloc(bufsize*sizeof(char));
    if (!buf) {
	perror("malloc");
	return errno;
    }
    size_t len=0;
    while((len=fread(buf,1,bufsize,fin)) > 0 ) {
	if (fwrite(buf,1, len, fout) != len) {
	    perror("ERROR: write");
	    int err=errno;
	    print_error("Scrivendo il file in output %s : errno=%d\n", outfile, errno);
	    return err;
	}
    }
    fclose(fin);
    fflush(fout); // flush del buffer in user space
    // scommentando la chiamata successiva faccio il flush anche dei buffer in kernel space
    //SYSCALL_EXIT(fsync,len,fsync(fileno(fout)),"Facendo il flush del file in output %s : errno=%d\n", outfile, errno);
    fclose(fout);
    free(buf);

    return 0;
}
