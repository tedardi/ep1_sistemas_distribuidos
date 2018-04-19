/*
** Este exemplo, correspondente ao 3.7 do capitulo 03 do livro do Alex Vrenios,comunica-se com subprocesos
** através do TCP.
*/

// contem definicoes e estruturas para a comunicacao IP
#include <netinet/in.h>
// contem as implementacoes das estruturas dos sockets
#include <sys/socket.h>
// contem as definições dos tipos de dados usados nas chamadas de sistema
#include <sys/types.h>
#include <sys/un.h>
// contem as definicoes do tipo porta e ip que sao descritas em <netinet/in.h>.
#include <netdb.h>
// padrao para inputs e outputs
#include <stdio.h>
// usada por causa do metodo memset
#include <string.h>
// adicionada pela funcao exit
#include <stdlib.h>
#include <sys/wait.h>
// adicionada por conhecer o metodo close() de sockets
#include <unistd.h>

char path[] = {"/tmp/socket3.3.7"}; /* socket name */

int main(void)
/*
**	Listing 3.7.c - TCP communication with forked subtask
*/
{
	struct sockaddr_un sock;
	int len = sizeof (sock);
	int pid; /* id do processo filho */
	int soc; /* arquivo de descricao do socket */
	char buffer[80];

	/* estabelece e inicializa um socket TCP */
	if((soc = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
	{
		perror("Listing 3.7: socket()");
		exit(-1);
	}

	memset((char *) &sock, 0, sizeof(sock));
	strcpy(sock.sun_path, path);
	sock.sun_family = AF_UNIX;

	/* cria um subprocesso filho */
	if((pid = fork ()) == 0)
	{
		/* publica o nome do socjet quando o escuta */
		bind(soc, (struct sockaddr *) &sock, len);

		/* aceita a conexao */
		if(listen (soc, 5) <0)
		{
			perror("Child: listen()");
			exit(-1);
		}
		if((soc = accept(soc, (struct sockaddr *) &sock, &len)) <0)
		{
			perror("child: accept()");
			exit(-1);
		}

		/* echo messages received from parent back to user */
		while(1)
		{
			if(recv(soc, buffer, sizeof(buffer), 0) < 0)
			{
				perror("child: %s, recv()");
				break;
			}
			// printf("child: %s, buffer"); -- original
			printf("child: %s", buffer); //provavelmente um erro de aspas no original
			if(strncmp(buffer, "EXIT", 4) == 0) 	/* exit request */
			{
				unlink(path);
				puts("Bye!");
				close(soc);
				break;
			}
		}

		/* Child exit */
		exit(0);
	}

	/* connect to server socket created by our child subtask */
	sleep(2); /* wait for setup */
	if(connect(soc, (struct sockaddr *) &sock, sizeof(sock)) <0)
	{
		perror("parent: connect()");
		exit(-1);
	}

	/* send a message typed from interactived user to child subtask */
	while(1)
	{
		sleep(1); /* response time */
		printf("\nEnter a message: ");
		fflush(stdout); /* limpa o buffer */
		fgets(buffer, sizeof(buffer), stdin);

		if(send(soc, buffer, strlen(buffer)+1, 0) < 0)
		{
			perror("parent: send()");
			exit(-1);
		}
		if(strncmp(buffer, "EXIT", 4) == 0) /* exit request */
		{
			close(soc);
			break;
		}
	}
	/* await Child exit. */
	waitpid(pid, NULL, 0);
	return 0;
}
