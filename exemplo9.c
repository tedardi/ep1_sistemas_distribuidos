#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>

#define PORT 5678

main()
/*
** Listing 3.8b.c- prints UDP datagrams from a remote client
*/
{
	struct sockaddr_in sock;
	int len = sizeof(sock);
	struct hostent *hp;
	int port = PORT;
	int fd;
	char buffer[80];

	/* establish and initialize UDP socket struct */
	if((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("server: socket()");
		exit(-1);
	}
	memset((char*) &sock, 0, sizeof(sock));
	sock.sin_addr.s_addr = htonl(INNADR_ANY);
	sock.sin_port = htons(port);
	sock.sin_family = AF_INET;
	if(bind(fd, (struct sockaddr *) &sock, len) < 0)
	{
		perror("server: bind()");
		close(fd);
		exit(-1);
	}
	
	/* echo messages received from client back to user */
	while(1)
	{
		recvfrom(fd, buffer, sizeof(buffer), 0, (struct sockaddr *) &sock, &len);
		printf("Server: %s", buffer);
		if(strncmp(buffer, "EXIT", 4) == 0) /* exit request */
		{
			puts("Bye!");
			close(fd);
			break;
		}
	}
	return(0);
}