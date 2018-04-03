#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Adicionad int no main, 0 no return, stdlib e sys/wait + corrigido argumentos do waitpid

char path[] = {"/tmp/socket3.3.6"}; /* socket name */

int main (void)
/*
** Listing3.6.c - UDP communication with forked subtask
*/
{
	struct sockaddr_un sock;
	int len = sizeof(sock);
	int pid; /* child task's process id */
	int fd; /* socket file descriptor */
	char buffer[80];

	/* establish and initialize UDP socket strucut */
	fd = socket(AF_UNIX, SOCK_DGRAM, 0);
	memset((char *) &sock, 0, sizeof(sock));
	strcpy(sock.sun_path, path);
	sock.sun_family = AF_UNIX;

	/* create child subtask */
	if((pid = fork()) == 0)
	{
		/* publish the port number we are listening to */
		bind(fd, (struct sockaddr *) &sock, len);

		/* echo messages received from parent back to user */
		while(1)
		{
			recvfrom(fd, buffer, sizeof(buffer), 0, (struct sockaddr *) &sock, &len);
			printf("Child: %s", buffer);
			if(strncmp(buffer, "EXIT", 4) == 0) /* exit request */
			{
				unlink(path);
				puts("Bye!");
				close(fd);
				break;
			}
		}

		/* Child exit */
		exit(0);
	}
	/* send a message typed from interactive user to child subtask */
	while(1)
	{
		sleep(1); /* response time */
		printf("\nEnter a message: \n");
		fflush(stdout);
		fgets(buffer, sizeof(buffer), stdin);
		sendto(fd, buffer, strlen(buffer)+1, 0, (struct sockaddr *) &sock, sizeof(sock));

		if(strncmp(buffer, "EXIT", 4) == 0) /* exit request */
		{
			close(fd);
			break;
		}
	}
	/* await Child exit */
	waitpid(pid, NULL, 0);
	return 0;
}