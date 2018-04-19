/*
RESUMO: Programa sempre faz o print do ID pai e do ID filho

No linux ID do pai sempre 1 digito menor que o filho: e.g 7058 (PAI) 7059 (FILHO). IDs nao se repetem
 
 Filho esta sempre executando dps do pai nos prints.
 o PID igual a zero aparece no print caso seja feito dentro do IF

Com o print do getpid() dentro do IF e um print do pid fora do if conseguimos ver exatamente o pai esperando o ID correto do filho.
Outra coisa interessante, como o filho ficou com o pid = 0, então dentro do if pid = 0, mas para o sistema operacional, ele tem um numero de processo, que esta no processo do pai
Por isso só o pai consegue identificar o filho

*/


//subtasking require a system call to create a indentical copy of your process

#include <stdio.h>

// Tive que adicionar stdlib pois método exit pertence a ela
#include <stdlib.h>

// Para que serve isso?
// Define os métodos getpid() e fork()
#include <unistd.h>

#include <sys/wait.h>

// Tive que adicionar int como retorno de main
int main(void) {
	// singles module subtasking using fork()
	int pid;
	printf("\nAtual PID do pai: %d\n", getpid());

	// fork cria um copia identica do pai
	// nesse caso estamos verificando se fork esta retornando 0, o que representa sucesso
	// Só podemos continuar caso seja 0, pois se nao for, a subtask nao foi criada com sucesso
	
	// Pelo que estou entendendo, na hora que damos fork() o programa divide em dois
	// Em um dos casos PID = 0, entao esse é o FILHO, e ele nao executará o IF
	// Em outro caso PID != de zero, então representa o PAI

	if((pid = fork()) == 0) {
		// Esse bloco só é executado pela subtask
		printf("PID: %d\n",pid);
		printf("Atual PID do filho: %d\n", getpid());
		sleep(10);
		printf("Filho agora morre\n");
		// Finalizamos o processo filho
		exit(0); 
	}
	printf("PAI esperando: %d\n",pid);
	// Esperamos o PID que presenta o filho
	// Tive que adicionar NULL e 0; NULL pois não importa o status que o filho retornou o 0 pois naõ quero modificar nenhuma opçõa. Seria interessante ir mais a fundo apra entender esses argumentos
	waitpid(pid, NULL, 0);
	printf("Pai morreu\n");
	// Tive que adicionar 0 no return devido ao tipo de retorno do método main
	return 0;
}
