#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>

#define PORT 4321

main(void)
/*
** Listing 3.9b.c - prints TCP packets from a remote client
*/
{
	struct sockaddr_in sock;
	int len = sizeof(sock);
	struct hostent *hp;
	int port = PORT;
	int acc, soc;
	char buffer[80];

	/* create a server socket to accept a connection */
	if((acc = socket(AF_INET, SOCK_STREAM, 0)) <0)
	{
		perror("server: socket()");
		exit(-1);
	}
	memset((char *) &sock, 0, sizeof(sock));
	sock.sin_addr.s_addr = htonl(INADDR_ANY);
	sock.sin_port = htons(port);
	sock.sin_family = AF_INET;
	if(bind(acc, (struct sockaddr *) &sock, len) <0)
	{
		perror("server:bind()");
		close(acc);
		exit(-1);
	}

	/* await connection */
	if(listen(acc, 5) <0)
	{
		perror("server: listen()");
		close(acc);
		exit(-1);
	}
	if((soc = accept(acc, (struct sockaddr *) &sock, &len)) <0)
	{

		perror("server: accept()");
		close(acc);
		exit(-1);
	}
	close(acc);

	/* echo messages received from client back to user */
	while(1)
	{
		recv(soc, buffer, sizeof(buffer), 0);
		printf("Server: %s, buffer");
		if(strncmp(buffer, "EXIT", 4) == 0) /* exit request */
		{
			puts("Bye!");
			close(soc);
			break;
		}
	}
	return(0);
}
