										/* Sends TCP packets to a Remote Server*/

# include <netinet/in.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <string.h>
# include <netdb.h>
# include <stdio.h>

// Adicionad stdlib devido função exit
#include <stdlib.h>

// Adiciona a biblioteca que conhece método close() de sockets
#include <unistd.h>

# define PORT 4321

// int main (argc, argv) - original
int main (int argc, char *argv[])
	// char *argv[];
	// int argc;
	/*
	** Listing 3.9a.c - sends TPC packets to a remote server
	*/
	{
		struct sockaddr_in sock;
		struct hostent *hp;
		int port = PORT;
		int fd;
		char buffer[80];

		/* server */
		if(argc < 2)
		{
			// printf("\n\tUsage: %s <IP_Addr>\n\n" argv[0]); -- original. Sem virgula separando o argumento
			printf("\n\tUsage: %s <IP_Addr>\n\n", argv[0]); 
			exit(-1);

		}
		else {
			// printf("hellllllo2");
			// printf("IP digitado %s\n", argv[1]);	
		}
		
		/* get vitals regarding remote server */
		strncpy(buffer, argv[1], sizeof(buffer));
		// printf("BUFFER: %s\n",buffer);
		// if(hp = gethostbyname(buffer)) == NULL -- original
		if((hp = gethostbyname(buffer)) == NULL)
		{
			// printf("GET HOST:%s\n", gethostbyname(buffer));
			perror("client: gethostbyname()");
			exit(-1);
		}

		/* initialize the TCP stream socket structure */
		if((fd =  socket(AF_INET, SOCK_STREAM,0))<0)
		{
			perror("client: socket()");
			exit(-1);
		}
		memset((char *) &sock, 0, sizeof(sock));
		memcpy(&sock.sin_addr, hp -> h_addr, hp -> h_length);
		sock.sin_family = hp -> h_addrtype;
		sock.sin_port = htons(port);

		/* connect to remote server */
		// if (connect(fd, (struct sockaddr *) &sock, sizeof(sock) < 0) -- original com um parenteses a menos, mas precisa verificar aonde fica o parenteses faltando. Eu coloquei no final, mas nao tenho ctz
		// if (connect(fd, (struct sockaddr *) &sock, sizeof(sock)) < 0)
		if (connect(fd, (struct sockaddr *) &sock, sizeof(sock) < 0))
		{
			perror ("client: connect()");
			exit(-1);
		}

		/* send a message typed from interactive user to child subtask */
		while(1)
		{
			printf("\nEnter a message: ");
			fflush(stdout);
			fgets(buffer, sizeof(buffer), stdin);

			if(send(fd, buffer, strlen(buffer)+1, 0) <0)
			{
				perror("parent: send()");
				// Adicionado ponto e virgula
				exit(-1);
			}

			// if(strncmp(buffer "EXIT", 4) == 0) /* exit request */ - original sem virgula entre buffer e exit
			if(strncmp(buffer ,"EXIT", 4) == 0) /* exit request */
			{
				close(fd);
				break;
			}
		}
		close(fd);
		return(0);
	}

