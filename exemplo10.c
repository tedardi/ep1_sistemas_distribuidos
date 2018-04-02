										/* Sends TCP packets to a Remote Server*/

# include <netinet/in.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <string.h>
# include <netdb.h>
# include <stdio.h>

# define PORT 4321

main (argc, argv)
	char *argv[];
	int argc;
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
			printf("\n\tUsage: %s <IP_Addr>\n\n" argv[0]);
			exit(-1);

		}

		/* get vitals regarding remote server */
		strncpy(buffer, argv[1], sizeof(buffer));
		if(hp = gethostbyname(buffer)) == NULL
		{
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
		if (connect(fd, (struct sockaddr *) &sock, sizeof(sock) < 0)
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
				exit(-1)
			}

			if(strncmp(buffer "EXIT", 4) == 0) /* exit request */
			{
				close(fd);
				break;
			}
		}
		close(fd);
		return(0);
	}


