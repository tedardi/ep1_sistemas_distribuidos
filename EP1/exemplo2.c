/*

Maior lição desse exemplo - o PID é referente ao OS e não ao programa, então mesmo qd damos exec, o PID do programa é passado ao filho e o waitpid() funciona esperando o outro programa acabar

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

// Não esta funcionando como deveria

int main(void) {
	// usando fork() e execl()

	int pid;
	printf("\nAtual PID do pai: %d\n", getpid());

	if((pid = fork()) == 0) {
		// Child task replaces itself with disk image of child
		// Pesquisar função execl
		// Requer um executável - mas nesse caso como vamos saber qd o pai morre? qd o executal termina?
		// execl("./exemplo2b", "exemplo2b", NULL);
		int someInt = getpid();
		char str[12];
		sprintf(str, "%d", someInt);
		// execvp("./exemplo2b", argv);
		execl("./exemplo2b", str, NULL);
		// execv("./exemplo2b", NULL);
		printf("Um execl de sucesso nunca irá retornar\n");
		exit(-1);
	}

	printf("Pai aguardando processo: %d\n", pid);
	waitpid(pid, NULL, 0);
	printf("Pai terminou aguardar\n");
	return 0;
}