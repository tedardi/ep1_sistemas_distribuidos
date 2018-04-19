/*
** Este exemplo, correspondente ao 3.9a do capitulo 03 do livro do Alex Vrenios, envia pacotes TCP para um servidor
** remoto. 
*/

// contem definicoes e estruturas para a comunicacao IP
# include <netinet/in.h> 
// contem as implementacoes das estruturas dos sockets
# include <sys/socket.h> 
// contem as definições dos tipos de dados usados nas chamadas de sistema
# include <sys/types.h>
// usada por causa do metodo memset
# include <string.h>
// contem as definicoes do tipo porta e ip que sao descritas em <netinet/in.h>.
# include <netdb.h>
// padrao para inputs e outputs
# include <stdio.h>
// ADICIONADA pela funcao exit
# include <stdlib.h>
// ADICIONADA por conhecer o metodo close() de sockets
# include <unistd.h>

/* As portas são definidas em 16bits, ou seja, existem as portas de 0 a 65535.
** Entretanto, as portas 0 ate 1999 sao reservadas para processos conhecidos e padronizados.
** Aconselha-se quando criar o processo, escolher uma porta entre 2000 a 65535.
*/
# define PORT 4321

// int main (argc, argv) - original
int main (int argc, char *argv[])
	// char *argv[];
	// int argc;

/*
** ***************************************************************************************************************
** Nesta primeira parte do codigo, cria-se as estruturas necessarias para a comunicacao e verifica a chamada correta
** ao servidor.
** ***************************************************************************************************************
*/


/*
** Inicialmente, ele inicia o código declarando o socket da biblioteca <sys/socket.h>
** Uma comunicação UDP precisa apenas da porta e do IP para ser estabelecida.
** Observe como essa estrutura é similar tanto ao cliente quanto ao servidor. 
*/

	{
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
		struct hostent *hp;
		// definida acima 
		int port = PORT; 
		// retorno a conexcao do socket pelo cliente, usado para encerrar o processo em caso de erro.
		int fd; 
		// o buffer permite mensagens de 80 caracteres. Aqui testamos mensagens maiores.
		char buffer[80]; 

		/* server */

		// Aqui, ele verifica se na chamada do sistema foram adicionadas todos os argumentos. Em especial se 
		// o IP não foi enviado na chamada.
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
		// o métoddo 'perror' é chamada caso uma chamada de sistema falhe, mostrando a mensagem de erro ao usuario.
		if((hp = gethostbyname(buffer)) == NULL)
		{
			// printf("GET HOST:%s\n", gethostbyname(buffer));
			perror("client: gethostbyname()");
			exit(-1);
		}

		/* initialize the TCP stream socket structure */
		// o métoddo 'perror' é chamada caso uma chamada de sistema falhe, mostrando a mensagem de erro ao usuario.
		// AF_INET define conexao por IPv4, caso fosse AF_INET6, exigira IPv6 para funcionar. 
		if((fd =  socket(AF_INET, SOCK_STREAM,0))<0)
		{
			perror("client: socket()");
			exit(-1);
		}


		// Aqui atribuo as variaveis ao socket
		memset((char *) &sock, 0, sizeof(sock));
		memcpy(&sock.sin_addr, hp -> h_addr, hp -> h_length);
		sock.sin_family = hp -> h_addrtype;
		sock.sin_port = htons(port);

		/* conexao com o servidor remoto */
		// if (connect(fd, (struct sockaddr *) &sock, sizeof(sock) < 0) -- original com um parenteses a menos, mas precisa verificar aonde fica o parenteses faltando. Eu coloquei no final, mas nao tenho ctz
		// if (connect(fd, (struct sockaddr *) &sock, sizeof(sock)) < 0)
		if (connect(fd, (struct sockaddr *) &sock, sizeof(sock) < 0))
		{
			perror ("client: connect()");
			exit(-1);
		}


/*
** ***************************************************************************************************************
** Nesta segunda parte do código, a conexão do TCP já foi implementada. Permite a interação dos pacotes recebidos
** do(s) cliente(s) com o servidor.
** ***************************************************************************************************************
*/

		/* send a message typed from interactive user to child subtask */

		while(1)
		{
			printf("\nEnter a message: ");
			fflush(stdout); /* limpa o buffer */
			fgets(buffer, sizeof(buffer), stdin);

			if(send(fd, buffer, strlen(buffer)+1, 0) <0)
			{
				perror("parent: send()");
				// Adicionado ponto e virgula
				exit(-1);
			}

			// if(strncmp(buffer "EXIT", 4) == 0) /* exit request */ - original sem virgula entre buffer e exit
			// Aqui, a condicional permite o cliente finalizar a sua comunicação com o servidor pela palavra EXIT
			if(strncmp(buffer ,"EXIT", 4) == 0) /* exit request */
			{
				close(fd);
				break;
			}
		}
		close(fd);
		return(0);
	}


