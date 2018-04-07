/*
** Este exemplo, correspondente ao 3.8b do capitulo 03 do livro do Alex Vrenios, recebe os pacotes UDP do cliente
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
#define PORT 5678

int main(void)

/*
** ***************************************************************************************************************
** Nesta primeira parte do codigo, cria-se as estruturas necessarias para receber os pacotes do cliente pelo servidor.
** ***************************************************************************************************************
*/

{

/*
** Inicialmente, ele inicia o código declarando o socket da biblioteca <sys/socket.h>
** Uma comunicação UDP precisa apenas da porta e do IP para ser estabelecida.
*/
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
	memset((char*) &sock, 0, sizeof(sock)); /* Esta linha de codigo nao entendi */
	// sock.sin_addr.s_addr = htonl(INNADR_ANY);-- original, provavelmente tem typo


	sock.sin_addr.s_addr = htonl(INADDR_ANY);
	sock.sin_port = htons(port);
	sock.sin_family = AF_INET;
	if(bind(fd, (struct sockaddr *) &sock, len) < 0)
	{
		perror("server: bind()");
		close(fd);
		exit(-1);
	}


/*
** ***************************************************************************************************************
** Nesta segunda parte do código, a conexão do UDP já foi executada. Imprime os pacotes recebidos
** do(s) cliente(s).
** ***************************************************************************************************************
*/
	/* echo messages received from client back to user */
	/*
	** No bloco a seguir, há um loop que se encessará apenas com a mensagem "EXIT".
	** Neste loop, o servidor está procurando possíveis mensagens de sockets UDP. Ele usa um buffer
	** para guardar essas mensagens.
	** Aqui pode ser adicionada qualquer intervenção o processamento mais simples do servidor para o cliente.
	*/
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