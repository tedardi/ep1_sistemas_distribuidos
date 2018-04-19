/*
** Este exemplo, correspondente ao 3.9b do capitulo 03 do livro do Alex Vrenios, recebe os pacotes TCP do cliente
** e os disponibiliza no terminal. 
*/

// contem definicoes e estruturas para a comunicacao IP
#include <netinet/in.h>
// contem as implementacoes das estruturas dos sockets
#include <sys/socket.h>
// contem as definicoes do tipo porta e ip que sao descritas em <netinet/in.h>.
#include <netdb.h>
// padrao para inputs e outputs
#include <stdio.h>
// ADICIONADA pela funcao exit
#include <stdlib.h>
// usada por causa do metodo memset
#include <string.h>
// ADICIONADA por conhecer o metodo close() de sockets
#include <unistd.h>

/* As portas são definidas em 16bits, ou seja, existem as portas de 0 a 65535.
** Entretanto, as portas 0 ate 1999 sao reservadas para processos conhecidos e padronizados.
** Aconselha-se quando criar o processo, escolher uma porta entre 2000 a 65535.
*/
#define PORT 4321

int main(void)
{

/*
** ***************************************************************************************************************
** Nesta primeira parte do codigo, cria-se as estruturas necessarias para receber os pacotes do cliente pelo servidor.
** ***************************************************************************************************************
*/

	// estrutura do socket conforme <sys/socket.h>, definida na implementacao como:  
		/*
		** struct sockaddr_in
		** {
  		** short   sin_family; precisa ser AF_INET
  		** u_short sin_port;
  		** struct  in_addr sin_addr;
  		** char    sin_zero[8];
		**	};
		*/
	struct sockaddr_in sock;
<<<<<<< HEAD:EP1/exemplo11.c
	memset((char *) &sock, 0, sizeof(sock));
=======
	memset((char*) &sock, 0, sizeof(sock));
>>>>>>> 52b883e554da9ca6661ea66c09d69b41d567a7df:exemplo11.c
	int len = sizeof(sock);
	struct hostent *hp;
	int port = PORT;
	int acc, soc;
	char buffer[80];

	/* Cria o socket para suportar conexão usando IPv4 */
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

	/* aguarda a conexão */
	if(listen(acc, 5) <0)
	{
		perror("server: listen()");
		close(acc);
		exit(-1);
	}

	/* verifica o aceite da conexão */
	if((soc = accept(acc, (struct sockaddr *) &sock, &len)) <0)
	{

		perror("server: accept()");
		close(acc);
		exit(-1);
	}
	close(acc);


/*
** ***************************************************************************************************************
** Nesta segunda parte do código, a conexão do TCP já foi executada. Imprime os pacotes recebidos
** do(s) cliente(s).
** ***************************************************************************************************************
*/

	// o while 1 cria um loop que termian apenas caso a condicional do if seja TRUE.
	while(1)
	{
		recv(soc, buffer, sizeof(buffer), 0);
		printf("Server: %s", buffer);
		// if(strncmp(buffer "EXIT", 4) == 0) /* exit request */ - original sem virgula entre buffer e exit
		// Aqui, a condicional permite o servidr finalizar a sua comunicação caso o cliente envie a palavra EXIT
		if(strncmp(buffer, "EXIT", 4) == 0) /* exit request */
		{
			puts("Bye!");
			close(soc);
			break;
		}
	}

	return(0);
}
