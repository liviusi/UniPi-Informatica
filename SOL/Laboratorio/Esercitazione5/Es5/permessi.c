#define _POSIX_C_SOURCE  200112L  // needed for S_ISSOCK
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <util.h>


void permessi(const char filename[]) {
    struct stat statbuf;
    int r;
    SYSCALL_EXIT(stat,r,stat(filename,&statbuf),"Facendo stat del nome %s: errno=%d\n",
	    filename, errno);


    char mode[11] = {'-','-','-','-','-','-','-','-','-','-','\0'};
    if (S_ISDIR(statbuf.st_mode)) mode[0]='d';
    if (S_ISCHR(statbuf.st_mode)) mode[0]='c';
    if (S_ISBLK(statbuf.st_mode)) mode[0]='b';
    if (S_ISLNK(statbuf.st_mode)) mode[0]='l';
    if (S_ISFIFO(statbuf.st_mode)) mode[0]='p';
    if (S_ISSOCK(statbuf.st_mode)) mode[0]='s';
    


    if (S_IRUSR & statbuf.st_mode) mode[1]='r';
    if (S_IWUSR & statbuf.st_mode) mode[2]='w';
    if (S_IXUSR & statbuf.st_mode) mode[3]='x';
    
    if (S_IRGRP & statbuf.st_mode) mode[4]='r';
    if (S_IWGRP & statbuf.st_mode) mode[5]='w';
    if (S_IXGRP & statbuf.st_mode) mode[6]='x';
    
    if (S_IROTH & statbuf.st_mode) mode[7]='r';
    if (S_IWOTH & statbuf.st_mode) mode[8]='w';
    if (S_IXOTH & statbuf.st_mode) mode[9]='x';
    
       
    struct passwd *pw = getpwuid(statbuf.st_uid);
    struct group  *gr = getgrgid(statbuf.st_gid);
    fprintf(stdout, "%-20s (%-7u): %-10ld  %-s  %-s,%-s  %-s", filename, (unsigned)statbuf.st_ino, statbuf.st_size, mode, pw->pw_name, gr->gr_name, ctime(&statbuf.st_mtime));		
}


int main(int argc, char * argv[])  {
    if (argc == 1) {
	fprintf(stderr, "usa: %s dir [dir]\n", argv[0]);
	return EXIT_FAILURE;
    }
    for(int i=1;i<argc;++i) permessi(argv[i]);

    return 0;
}
