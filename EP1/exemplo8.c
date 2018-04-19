/*
** Este exemplo, correspondente ao 3.8a do capitulo 03 do livro do Alex Vrenios, envia pacotes UDP para um servidor
** remoto. 
*/

// contem definicoes e estruturas para a comunicacao IP
#include <netinet/in.h>
// contem as implementacoes das estruturas dos sockets
#include <sys/socket.h>
// contem as definições dos tipos de dados usados nas chamadas de sistema
#include <sys/types.h>
// usada por causa do metodo memset
#include <string.h>
// contem as definicoes do tipo porta e ip que sao descritas em <netinet/in.h>.
#include <netdb.h>
// padrao para inputs e outputs
#include <stdio.h>
// ADICIONADA pela funcao exit
#include <stdlib.h>
// ADICIONADA por conhecer o metodo close() de sockets
#include <unistd.h>

/* As portas são definidas em 16bits, ou seja, existem as portas de 0 a 65535.
** Entretanto, as portas 0 ate 1999 sao reservadas para processos conhecidos e padronizados.
** Aconselha-se quando criar o processo, escolher uma porta entre 2000 a 65535.
*/
#define PORT 5678

int main(int argc, char *argv[])
// char *argv[];
// int argc; - original, mas declaração fora do escopo da função não é mais interpretado por muitos compiladores devido dificultar leitura do código

{
	// char *argv[];
	// int argc;
	/*
** Inicialmente, ele inicia o código declarando o socket da biblioteca <sys/socket.h>
** Uma comunicação UDP precisa apenas da porta e do IP para ser estabelecida.
** Observe como essa estrutura é similar tanto ao cliente quanto ao servidor. 
*/
	struct sockaddr_in sock;
	struct hostent *hp;
	int port = PORT;
	int fd;
	char buffer[80];

	// Aqui, ele verifica se na chamada do sistema foram adicionadas todos os argumentos. Em especial se 
	// o IP não foi enviado na chamada.
	/* server */
	if(argc < 2)
	{
		printf("\n\tUsage: %s <IP_Addr>\n\n", argv[0]);
		exit(-1);
	}

	/* get vitals regarding remote server */
	/* aqui envia os dados escritos para o cliente, armazenando para um buffer e enviando como argumento 
	*/
	// o métoddo 'perror' é chamada caso uma chamada de sistema falhe, mostrando a mensagem de erro ao usuario.

	strncpy(buffer, argv[1], sizeof(buffer));
	if((hp = gethostbyname(buffer)) == NULL)
	{
		perror("client: gethostbyname()");
		exit(-1);
	}
	
	/* establish and initialize the UDP socket */
	if((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("client: socket()");
		exit(-1);
	}


	// Aqui atribuo as variaveis ao socket
	memset((char *) &sock, 0, sizeof(sock)); 
	memcpy(&sock.sin_addr, hp -> h_addr, hp->h_length);
	sock.sin_family = hp->h_addrtype;
	sock.sin_port = htons(port);

	/* envio uma mensagem digitado pelo usuario para o servidor */
	while(1)
	{
		printf("\nEnter a message: ");
		fflush(stdout); /* limpa o buffer */
		fgets(buffer, sizeof(buffer), stdin);

		sendto(fd, buffer, strlen(buffer)+1, 0, (struct sockaddr *) &sock, sizeof(sock));
		/* esta linha envia a mensagem, de fato, para o servidor. */

		if(strncmp(buffer, "EXIT", 4) == 0) /* exit request */
		{
			close(fd);
			break;
		}
	}
	puts("Bye!");
	return(0);
}
