// Signals = eventos que acontecem com tanta frequencia que são codificados numericamente

/*

Ver como a troca de sinais entre processos funciona, mostrar como processos podem interagir por sinais
*/

#include <sys/signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

// Qual a diferença entre signal e sys/signal?
// #include <signal.h>

int count = 0; // contador para eventos ^C

// Tive que inveter a ordem da função para que control_C fosse reconhecido
void control_C(int sig) {
	// Signal cria um sinal no sistema assim que o argumento foi verdadeiro, nesse caso, um control_C	
	
	// signal(SIGINT, control_C);
	count++;
	// Mostrar o tipo de sinal
	printf("pegou sinal %d\n", sig);
	exit(1);
}

int main(void) {

	int pid;
	// Lida com sinais ^C digitados no teclado
	signal(SIGINT, control_C);

	// kill(pid, SIGNAL) - mandar sinal / só recomendado para pai filho devido a condições herediatária de "race conditions"
	printf("getpid %d\n", getpid());

	if((pid = fork()) == 0) {
		// Child task replaces itself with disk image of child
		// Pesquisar função execl
		// Requer um executável - mas nesse caso como vamos saber qd o pai morre? qd o executal termina?
		// execl("./exemplo2b", "exemplo2b", NULL);
		printf("Ola eu sou o filho\n");
		int contador = 0;
		while(1) {
			printf("filho executando.....\n");
			contador++;
			sleep(1);
			if(contador == 100) {
				exit(0);
			}
		}
		exit(0);
	}

	printf("PAI esperando: %d\n",pid);
	while(1) {
		sleep(3);
		printf("Parando o filho\n");
		kill(pid, 19);
		sleep(3);
		printf("Retomando o filho\n");
		kill(pid, 18);
	}

	while(1) {
      		printf("Going to sleep for a second...\n");
      		sleep(1); 
   	}

	while(count < 3) {
		printf("Digite ^C... ");
		fflush(stdout);
		pause();
		printf("Lidado com eventos #%d\n", count);
	}
	printf("3 eventos foram lidados\n");
}

// Para testar precisa de uma outra janela (diferente processo) usar:

// kill -2 3721 (IDPROCESSO)
// caso seja um outro programa kill(2, 3721);

// Genericos, para testar:
// 10 - SIGUSR1
// 12 - SIGUSR2