#define _POSIX_C_SOURCE  200112L
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <utilities.h>

#define SOCKETNAME "Esercizio3/socket"
#define SOCKETMAXQUEUE 16

/**
 * Questa soluzione e' quella fornita dai professori del corso
 * ed e' stata utilizzata per capire come funzionasse la chiamata
 * di select e quale fosse il suo uso.
*/

/** 
 * tipo del messaggio
 */
typedef struct msg
{
	int len;
	char *str;
} msg_t;


void cleanup()
{
	unlink(SOCKETNAME);
}

// converte tutti i carattere minuscoli in maiuscoli
void toup(char *str)
{
	char *p = str;
	while(*p != '\0')
	{ 
		*p = (islower(*p)?toupper(*p):*p); 
		++p;
	}
}

int cmd(long connfd)
{
	msg_t str;
	if (readn(connfd, &str.len, sizeof(int)) <= 0) return -1;
	str.str = calloc((str.len), sizeof(char));
	if (!str.str)
	{
		perror("calloc");
		fprintf(stderr, "Memoria esaurita....\n");
		return -1;
	}
	if (readn(connfd, str.str, str.len*sizeof(char)) <= 0) return -1;
	toup(str.str);
	if (writen(connfd, &str.len, sizeof(int)) <= 0)
	{
		free(str.str);
		return -1;
	}
	if (writen(connfd, str.str, str.len*sizeof(char)) <= 0)
	{
		free(str.str);
		return -1;
	}
	free(str.str);
	return 0;
}

// ritorno l'indice massimo tra i descrittori attivi
int updatemax(fd_set set, int fdmax)
{
	for(int i = (fdmax - 1); i >= 0; i--)
		if (FD_ISSET(i, &set)) return i;
	assert(1==0);
	return -1;
}

int main(int argc, char *argv[])
{
	int listenfd;
	EXIT_IF_EQ(listenfd, -1, socket(AF_UNIX, SOCK_STREAM, 0), socket);

	struct sockaddr_un serv_addr;
	memset(&serv_addr, '0', sizeof(serv_addr));
	serv_addr.sun_family = AF_UNIX;    
	strncpy(serv_addr.sun_path, SOCKETNAME, strlen(SOCKETNAME)+1);
	int err;
	EXIT_IF_EQ(err, -1, bind(listenfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)), bind);
	EXIT_IF_EQ(err, -1, listen(listenfd, SOCKETMAXQUEUE), listen);

	fd_set set, tmpset;
	// azzero sia il master set che il set temporaneo usato per la select
	FD_ZERO(&set);
	FD_ZERO(&tmpset);

	// aggiungo il listener fd al master set
	FD_SET(listenfd, &set);

	// tengo traccia del file descriptor con id piu' grande
	int fdmax = listenfd; 

    for(;;)
	{      
		// copio il set nella variabile temporanea per la select
		tmpset = set;
		
		// From "Unix Network Programming: The Sockets Networking Api"
		// by R. Stevens et al.
		// The maxfdp1 (fdmax+1 ed) argument specifies the number of descriptors to be tested.
		// Its value is the maximum descriptor to be tested plus one (hence our
		// name of maxfdp1). The descriptors 0, 1, 2, up through and including
		// maxfdp1 1 are tested. The constant FD_SETSIZE, defined by including
		// <sys/select.h>, is the number of descriptors in the fd_set datatype.
		// Its value is often 1024, but few programs use that many descriptors.
		// The maxfdp1 argument forces us to calculate the largest descriptor that
		// we are interested in and then tell the kernel this value. For example,
		// given the previous code that turns on the indicators for descriptors
		// 1, 4, and 5, the maxfdp1 value is 6. The reason it is 6 and not 5 is
		// that we are specifying the number of descriptors, not the largest value,
		// and descriptors start at 0.	
		if (select(fdmax+1, &tmpset, NULL, NULL, NULL) == -1)
		{  // attenzione al +1
			perror("select");
			return -1;
		}
		// cerchiamo di capire da quale fd abbiamo ricevuto una richiesta
		for(int i=0; i <= fdmax; i++)
		{
			if (FD_ISSET(i, &tmpset))
			{
				long connfd;
				if (i == listenfd)
				{ // e' una nuova richiesta di connessione 
					SYSCALL_EXIT("accept", connfd, accept(listenfd, (struct sockaddr*)NULL ,NULL), "accept", "");
					FD_SET(connfd, &set);  // aggiungo il descrittore al master set
					if(connfd > fdmax) fdmax = connfd;  // ricalcolo il massimo
					continue;
				} 
				connfd = i;  // e' una nuova richiesta da un client già connesso
			
				// eseguo il comando e se c'e' un errore lo tolgo dal master set
				if (cmd(connfd) < 0)
				{ 
					close(connfd); 
					FD_CLR(connfd, &set); 
					// controllo se deve aggiornare il massimo
					if (connfd == fdmax) fdmax = updatemax(set, fdmax);
				}
			}
		}
	}

	EXIT_IF_EQ(err, -1, close(listenfd), close);
	EXIT_IF_EQ(err, -1, remove(SOCKETNAME), remove);
	return 0;
}