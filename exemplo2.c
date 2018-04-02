#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
	// usando fork() e execl()

	int pid;
	printf("\nAtual PID do pai: %d\n", getpid());

	if((pid = fork()) == 0) {
		// Child task replaces itself with disk image of child
		// Pesquisar função execl
		// Requer um executável - mas nesse caso como vamos saber qd o pai morre? qd o executal termina?
		execl("./exemplo2", "exemplo2", NULL);
		printf("Um execl de sucesso nunca irá retornar\n");
		exit(-1);
	}
	printf("Pai aguardando\n");
	waitpid(pid, NULL, 0);
	printf("Pai terminou aguardar\n");
	return 0;
}