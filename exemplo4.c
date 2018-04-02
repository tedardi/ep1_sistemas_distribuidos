// IMPORTANTE - se um processo filho muda o valor de uma variável, o pai nao pode ver com dois espacoss de endereçamento diferentes

// Se o filho precisa se comunicar com o pai, é necessário usar SHARED MEMORY

// Através de syscall é possível alocar shared memory, e pode ser modificado por qualquer processo a quem for dada permissão. 
// Memoria compartilhada não é só para subtask, é para qualquer outro processo (seria legal testar isso)

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
	// Um filho muda uma variável na memoria compartilhada do pai

	struct area { //area de memoria compartilhada
		int value; 
	} area, *ap;

	int flag = (IPC_CREAT | IPC_EXCL | 0660);
	int size = sizeof(struct area);
	// key_t key = 0x01020304; //exemplo

	// Só funcionou a 2ª vez qd mudei o numero do key
	key_t key = 0x02030404; //exemplo
	int shmid; //ID da area de memoria compartilhada
	int pid; //id do processo filho

	// Recuperar uma área de memoria compartilhada
	shmid = shmget(key, size, flag);

	// Anexar uma area de memoria compartilhada
	ap = (struct area *) shmat(shmid, 0, 0);

	// Set de value of the shared memory data
	ap->value = 0; //inicializando o valor com 0
	printf("Valor inicial setado para 0 \n");

	if((pid = fork()) == 0) {

		// anexa a área de memoria compartilhada
		ap = (struct area *) shmat(shmid, 0, 0);
		// Repara que o shmid'ainda é colocado corretamente

		sleep(2); //deixar o pai esperar um pouco

		// mudando o valor na memoria compartilhada
		ap->value = 1;

		exit(0);
	}

	// fica de olho na mudança de valor
	while(1) {
		printf("Pai: valor = %d\n", ap->value);
		fflush(stdout);
		if(ap->value != 0)
			break;
		printf("continuando...\n");
		sleep(1);
	}
	printf("Bye\n");

	waitpid(pid, NULL, 0);

	// libera a area de memoria compartilhada
	shmctl(shmid, IPC_RMID, (struct shmid_ds *) 0);

	return 0;
}

// Tentar rodar sem o sleep 