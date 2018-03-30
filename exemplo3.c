// Signals = eventos que acontecem com tanta frequencia que são codificados numericamente

#include <sys/signal.h>
#include <stdio.h>
#include <unistd.h>

int count = 0; // contador para eventos ^C

main() {
	// Lida com sinais ^C digitados no teclado
	signal(SIGINT, control_C);

	while(count < 3) {
		printf("Digite ^C... ");
		fflush(stdout);
		pause();
		printf("Lidado com eventos #%d\n", count);
	}
	printf("3 eventos foram lidados\n");
}

void control_C() {
	signal(SIGINT, control_C);
	count++;
}

// Para testar precisa de uma outra janela (diferente processo) usar:

// kill -2 3721 (IDPROCESSO)
// caso seja um outro programa kill(2, 3721);

// Genericos, para testar:
// 10 - SIGUSR1
// 12 - SIGUSR2