#include <stdio.h>
#include <unistd.h>

main() {
	printf("PID do processo filho %d\n", getpid());
	exit(0);
}

//FORK = identico, uma cópia exata do pai
// EXCECL = process é totalmente sobrescrito na memoria por um executável. Mas o que isso significa?

// Fork sempre retorna != 0 para o PAI e = 0 para o FILHO

// Comunicação entre processos é através do IPC