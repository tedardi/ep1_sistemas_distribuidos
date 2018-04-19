/*
** Este exemplo, correspondente ao 3.6 do capitulo 03 do livro do Alex Vrenios,comunica-se com subprocesos
** através do UDP.
*/


// contem definicoes e estruturas para a comunicacao IP
#include <netinet/in.h>
// contem as implementacoes das estruturas dos sockets
#include <sys/socket.h>
// contem as definições dos tipos de dados usados nas chamadas de sistema
#include <sys/types.h>
#include <sys/un.h>
#include <sys/wait.h>
// contem as definicoes do tipo porta e ip que sao descritas em <netinet/in.h>.
#include <netdb.h>
// padrao para inputs e outputs
#include <stdio.h>
// adicionada pela funcao exit
#include <stdlib.h>
// usada por causa do metodo memset
#include <string.h>
// adicionada por conhecer o metodo close() de sockets
#include <unistd.h>

// Adicionad int no main, 0 no return, stdlib e sys/wait + corrigido argumentos do waitpid

char path[] = {"/tmp/socket3.3.6"}; /* socket name */

int main (void)

{
	struct sockaddr_un sock;
	int len = sizeof(sock);
	int pid; /* child task's process id */
	int fd; /* socket file descriptor */
	char buffer[80];

	/* establish and initialize UDP socket strucut */
	fd = socket(AF_UNIX, SOCK_DGRAM, 0);
	memset((char *) &sock, 0, sizeof(sock));
	strcpy(sock.sun_path, path);
	sock.sun_family = AF_UNIX;

	/* create child subtask */
	if((pid = fork()) == 0)
	{
		/* publish the port number we are listening to */
		bind(fd, (struct sockaddr *) &sock, len);

		/* echo messages received from parent back to user */
		while(1)
		{
			recvfrom(fd, buffer, sizeof(buffer), 0, (struct sockaddr *) &sock, &len);
			printf("Child: %s", buffer);
			if(strncmp(buffer, "EXIT", 4) == 0) /* exit request */
			{
				unlink(path);
				puts("Bye!");
				close(fd);
				break;
			}
		}

		/* Child exit */
		exit(0);
	}
	/* send a message typed from interactive user to child subtask */
	while(1)
	{
		sleep(1); /* response time */
		printf("\nEnter a message: \n");
		fflush(stdout); /* limpa o buffer */
		fgets(buffer, sizeof(buffer), stdin);
		sendto(fd, buffer, strlen(buffer)+1, 0, (struct sockaddr *) &sock, sizeof(sock));

		if(strncmp(buffer, "EXIT", 4) == 0) /* exit request */
		{
			close(fd);
			break;
		}
	}
	/* await Child exit */
	waitpid(pid, NULL, 0);
	return 0;
}