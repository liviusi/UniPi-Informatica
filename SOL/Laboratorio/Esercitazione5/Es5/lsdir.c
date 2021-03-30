#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include <util.h>

#if !defined(MAXFILENAME)
#define MAXFILENAME 2048
#endif

int isdot(const char dir[]) {
  int l = strlen(dir);
  
  if ( (l>0 && dir[l-1] == '.') ) return 1;
  return 0;
}

void lsR(const char nomedir[]) {
    // controllo che il parametro sia una directory
    struct stat statbuf;
    int r;
    SYSCALL_EXIT(stat,r,stat(nomedir,&statbuf),"Facendo stat del nome %s: errno=%d\n",
	    nomedir, errno);

    DIR * dir;
    fprintf(stdout, "-----------------------\n");
    fprintf(stdout, "Directory %s:\n",nomedir);
    
    if ((dir=opendir(nomedir)) == NULL) {
	perror("opendir");
	print_error("Errore aprendo la directory %s\n", nomedir);
	return;
    } else {
	struct dirent *file;
    
	while((errno=0, file =readdir(dir)) != NULL) {
	    struct stat statbuf;
	    char filename[MAXFILENAME]; 
	    int len1 = strlen(nomedir);
	    int len2 = strlen(file->d_name);
	    if ((len1+len2+2)>MAXFILENAME) {
		fprintf(stderr, "ERRORE: MAXFILENAME troppo piccolo\n");
		exit(EXIT_FAILURE);
	    }	    
	    strncpy(filename,nomedir,      MAXFILENAME-1);
	    strncat(filename,"/",          MAXFILENAME-1);
	    strncat(filename,file->d_name, MAXFILENAME-1);
	    
	    if (stat(filename, &statbuf)==-1) {
		perror("eseguendo la stat");
		print_error("Errore nel file %s\n", filename);
		return;
	    }
	    if(S_ISDIR(statbuf.st_mode)) {
		if ( !isdot(filename) ) lsR(filename);
	    } else {
		char mode[10] = {'-','-','-','-','-','-','-','-','-','\0'};
		if (S_IRUSR & statbuf.st_mode) mode[0]='r';
		if (S_IWUSR & statbuf.st_mode) mode[1]='w';
		if (S_IXUSR & statbuf.st_mode) mode[2]='x';

		if (S_IRGRP & statbuf.st_mode) mode[3]='r';
		if (S_IWGRP & statbuf.st_mode) mode[4]='w';
		if (S_IXGRP & statbuf.st_mode) mode[5]='x';

		if (S_IROTH & statbuf.st_mode) mode[6]='r';
		if (S_IWOTH & statbuf.st_mode) mode[7]='w';
		if (S_IXOTH & statbuf.st_mode) mode[8]='x';
		
		fprintf(stdout, "%20s: %10ld  %s\n", file->d_name, statbuf.st_size, mode);		
	    }
	}
	if (errno != 0) perror("readdir");
	closedir(dir);
	fprintf(stdout, "-----------------------\n");
    }
}


int main(int argc, char * argv[])  {
    if (argc != 2) {
	fprintf(stderr, "usa: %s dir\n", argv[0]);
	return EXIT_FAILURE;
    }

    // controllo l'argomento
    struct stat statbuf;
    int r;
    SYSCALL_EXIT(stat,r,stat(argv[1],&statbuf),
		 "Facendo stat del nome %s: errno=%d\n",
		 argv[1], errno);
    if(!S_ISDIR(statbuf.st_mode)) {
	fprintf(stderr, "%s non e' una directory\n", argv[1]);
	return EXIT_FAILURE;
    }    

    lsR(argv[1]);
    return 0;
}
