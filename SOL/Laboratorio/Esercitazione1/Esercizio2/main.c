#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
 
const int REALLOC_INC=16;
 
void RIALLOCA(char** buf, size_t newsize) {
	// <inserire il codice per riallocare buf alla nuova size newsize>
	*buf = (char*) realloc(*buf, newsize);
}

char* mystrcat(char *buf, size_t sz, char *first, ...) {
	// <implementare il codice che se necessario chiamerà RIALLOCA>
	char* i = NULL;
	size_t acc = 0;
	va_list args;
	va_start (args, first);
	for(i = first; i != NULL; i = va_arg (args, char*))
	{
		if (strlen(i) >= (strlen(buf) - acc))
		{
			char* dummy = buf;
			RIALLOCA(&buf, strlen(buf) + strlen(i));
			buf = strncpy(buf, dummy, strlen(dummy));
		}
		acc += strlen(i);
		strcat(buf, i);
	}
	va_end (args);
	return buf;
}
 
int main(int argc, char *argv[]) {
	if (argc < 7)
	{
		printf("troppi pochi argomenti\n");
		return -1;
	}
	char *buffer=NULL;
	RIALLOCA(&buffer, REALLOC_INC);  // macro che effettua l'allocazione del 'buffer'
	buffer[0]='\0'; // mi assicuro che il buffer contenga una stringa
	buffer = mystrcat(buffer, REALLOC_INC, argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], NULL);
	printf("%s\n", buffer);
	free(buffer);
	return 0;
}
