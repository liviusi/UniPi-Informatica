#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


#if !defined(MAXFILENAME)
#define MAXFILENAME 2048
#endif

int printdir(const char nomedir[]) {
    // controllo che il parametro sia una directory
    struct stat statbuf;
    int r;
    if ((r=stat(nomedir,&statbuf)) == -1) {
	perror("stat");
	return -1;
    }
    if(S_ISDIR(statbuf.st_mode)) {
	DIR * dir;
	if ((dir=opendir(nomedir)) == NULL) {
	    perror("opendir");
	    return -1;
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
		    return -1;
		}
		char mode[11] = {'-','-','-','-','-','-','-','-','-','-','\0'};
		if(S_ISDIR(statbuf.st_mode)) {
		    mode[0]='d';
		}
		if (S_IRUSR & statbuf.st_mode) mode[1]='r';
		if (S_IWUSR & statbuf.st_mode) mode[2]='w';
		if (S_IXUSR & statbuf.st_mode) mode[3]='x';
		
		if (S_IRGRP & statbuf.st_mode) mode[4]='r';
		if (S_IWGRP & statbuf.st_mode) mode[5]='w';
		if (S_IXGRP & statbuf.st_mode) mode[6]='x';
		
		if (S_IROTH & statbuf.st_mode) mode[7]='r';
		if (S_IWOTH & statbuf.st_mode) mode[8]='w';
		if (S_IXOTH & statbuf.st_mode) mode[9]='x';

		fprintf(stdout, "%20s: %10ld  %s\n", file->d_name, statbuf.st_size, mode);		
	    }
	}
	if (errno != 0) perror("readdir");
	closedir(dir);	
    } else 
	fprintf(stderr, "%s non e' una directory\n", nomedir);
    return 0;    	      
}


int main(int argc, char * argv[])  {
    if (argc != 2) {
	fprintf(stderr, "usa: %s dir\n", argv[0]);
	return -1;
    }

    return printdir(argv[1]);
}
