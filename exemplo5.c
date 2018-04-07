// Cada semaforo esta alinhado indiretamente com uma shared memory

// Semaforo - simples
// Se estiver verde o OS deixa usar o recurso; Se estiver vermelho o OS te deixa on hold até o ultimo processo liberar o recurso

// Tb chamado de serialização de um recurso - todas as ações devem ter feitas uma de cada vez e em sequencia

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

// Adicionado manualmente wait e stdlib
#include <sys/wait.h>
#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main(void) {
	// Simultaneamente lidando com depositos e saques

	struct area { //área de compartilhamento de meḿória
		int balance;
		int deposit;
		int withdrawal;
	} area, *ap;

#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)\
	// Union semun é normalmente definido qd inclui sys/sem
	// printf("SEGUNDO TESTE\n");
#else
	// printf("TERCEIRO TESTE\n");
	// Se não, temos que definir nós mesmos usando bits/sem.hold
	union semun
	{
		int val; //valor para SETVAL
		struct semid_ds *buf; //buffer para IPC_STAT, IPC_SET
		unsigned short int *array; // array para GETALL, SETALL
		struct seminfo *__buf; //buffer para IPC_INFO
	};
#endif
	union semun un; //semaforo uniao e buffer
	struct sembuf buf;

	int flag = (IPC_CREAT | IPC_EXCL | 0660);
	int size = sizeof(struct area);
	srand(time(NULL));
	key_t skey = ftok("exemplo5", rand());
	// key_t skey = 0x03040501; //exemplos de chave
	key_t mkey = ftok("exemplo5", rand());

	int semid; //id semafoto
	int shmid; // id da memoria compartilhada
	int pidD; //pid do filho deposito
	int pidW; //pid do filho saque
	int amount;
	char inbuf[80];

	// recupera area de memoria compartilhada
	shmid = shmget(mkey, size, flag);

	// anexa a area de memoria compartilhada
	ap = (struct area *) shmat(shmid, 0, 0);

	// inicializa uma info de conta na memoria compartilhada
	ap->balance = 1000;
	ap->withdrawal = 0;
	ap->deposit = 0;

	// recupera e inicializa um semafoto
	semid = semget(skey, 1, flag);

	// seta semaforo como disponivel
	un.val = 1;
	semctl(semid, 0, SETVAL, un);

	// Primeiro e unico semaforo
	buf.sem_num = 0;
	// espera if ser setado
	buf.sem_flg = 0;

	if((pidD = fork()) == 0) { //DEPOSITOS
		// anexa memoria compartilhada do pai
		ap = (struct area *) shmat(shmid, 0 , 0);

		// lida com depositos
		while(1) {
			sleep(1);

			// set semaforo
			buf.sem_op = -1;
			semop(semid, (struct sembuf *) &buf, 1);

			if(ap->deposit < 0)
				break; 

			if(ap->deposit > 0) {
				// lida do deposito
				ap->balance += ap->deposit;
				ap->deposit = 0; //limpa o deposito
			}

			// Limpa semaforo
			buf.sem_op = 1;
			semop(semid, (struct sembuf *) &buf, 1);

		}

		// Filho termina a execução normalmente
		exit(0);
	}

	// Nao deveria ser pidW? - MUDADO para pidW apesar de no livro estar escrito pidD
	if((pidW = fork()) == 0) { //WITHDRAWALS

		// anexa memoria compartilhada do pai
		ap = (struct area *) shmat(shmid, 0 , 0);

		// lida com depositos
		while(1) {
			sleep(1);

			// set semaforo
			buf.sem_op = -1;
			semop(semid, (struct sembuf *) &buf, 1);

			if(ap->withdrawal < 0)
				break; 

			if(ap->withdrawal > 0) {
				// lida do deposito
				ap->balance -= ap->withdrawal;
				ap->withdrawal = 0; //limpa o deposito
			}

			// Limpa semaforo
			buf.sem_op = 1;
			semop(semid, (struct sembuf *) &buf, 1);

		}

		// Filho termina a execução normalmente
		exit(0);	

	}

	// Pai lida com as transações de  depositos e saques
	printf("\n\n\nWELCOME TO THE FIRST INTERACTIVE BANK\n\n");
	while(1) {
		printf("Digite D para deposito e W para saque\n");
		fflush(stdout);
		fgets(inbuf, sizeof(inbuf), stdin);
		
		// DEPOSITOS SELECIONADO
		if(inbuf[0] == 'D' || inbuf[0] == 'd') {
			printf("Total da conta: %d\n", ap->balance);
			printf("Digite o quanto deseja depositar \n");
			fflush(stdout);
			fgets(inbuf, sizeof(inbuf), stdin);

			// Seta semaforo
			buf.sem_op = -1;
			semop(semid, (struct sembuf *) &buf, 1);

			amount = atoi(inbuf);
			if(amount <= 0) { //Exit requested
				// signal subtask
				ap->deposit = -1;
				ap->withdrawal = -1;
				break;
			}
			ap->deposit = amount;

			// limpa semaforo
			buf.sem_op = 1;
			semop(semid, (struct sembuf *) &buf, 1);
		}
		else if (inbuf[0] == 'W' || inbuf[0] == 'w') { 

			printf("Total da conta: %d\n", ap->balance);
			printf("Digite o quanto deseja sacar \n");
			fflush(stdout);
			fgets(inbuf, sizeof(inbuf), stdin);

			// Seta semaforo
			buf.sem_op = -1;
			semop(semid, (struct sembuf *) &buf, 1);

			amount = atoi(inbuf);
			if(amount <= 0) { //Exit requested
				// signal subtask
				ap->deposit = -1;
				ap->withdrawal = -1;
				break;
			}
			else if(amount <= ap->balance) {
				ap->withdrawal = amount; //saca o valor
			}
			else {
				printf("ERRO: fundo insuficiente\n");
			}
			// limpa semaforo
			buf.sem_op = 1;
			semop(semid, (struct sembuf *) &buf, 1);
		}
		else {
			printf("Código invalido '%c'\n", inbuf[0]);
		}
	}

	// Espera filhos terminarem
	waitpid(pidD, NULL, 0);
	waitpid(pidW, NULL, 0);

	printf("Seu saldo final é de %d\n", ap->balance);

	// libera a area de memoria compartilhada
	shmctl(shmid, IPC_RMID, (struct shmid_ds *) 0);
	semctl(semid, 0, IPC_RMID, un);
	return 0;


}