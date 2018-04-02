//subtasking require a system call to create a indentical copy of your process

#include <stdio.h>

// Tive que adicionar stdlib pois método exit pertence a ela
#include <stdlib.h>

// Para que serve isso?
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
		printf("\nAtual PID do filho: %d\n", getpid());

		// Finalizamos o processo filho
		exit(0); 
	}

	// Esperamos o PID que presenta o filho
	// Tive que adicionar NULL e 0; NULL pois não importa o status que o filho retornou o 0 pois naõ quero modificar nenhuma opçõa. Seria interessante ir mais a fundo apra entender esses argumentos
	waitpid(pid, NULL, 0);
	
	// Tive que adicionar 0 no return devido ao tipo de retorno do método main
	return 0;
}
