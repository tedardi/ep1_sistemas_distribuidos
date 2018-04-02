#include <netinet/.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

char path[] = {"/tmp/socket3.3.7"}; /* socket name */

main(void)
/*
**	Listing 3.7.c - TCP communication with forked subtask
*/
{
	struct sockaddr_un sock;
	int len = sizeof (sock);
	int pid; /* child task's process id */
	int soc; /* socket file descriptor */
	char buffer[80];

	/* establish and initialize TCP socket struct */
	if((soc = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
	{
		perror("Listing 3.7: socket()");
		exit(-1);
	}
	memset((char *) &sock, 0, sizeof(sock));
	strcpy(sock.sun_path, path);
	sock.sun_family = AF_UNIX;

	/* create child subtask */
	if((pid = fork ()) == 0)
	{
		/* publish the socket name we listen to */
		bind(soc, (struct sockaddr *) &sock, len);

		/* accept connection */
		if(listen (soc, 5) <0)
		{
			perror("Child: listen()");
			exit(-1);
		}
		if((soc = accept(soc, (struct sockaddr *) &sock, &len)) <0)
		{
			perror("child: accept()");
			exit(-1);
		}

		/* echo messages received from parent back to user */
		while(1)
		{
			if(recv(soc, buffer, sizeof(buffer), 0) < 0)
			{
				perror("child: %s, recv()");
				break;
			}
			printf("child: %s, buffer");
			if(strncmp(buffer, "EXIT", 4) == 0) 	/* exit request */
			{
				unlink(path);
				puts("Bye!");
				close(soc);
				break;
			}
		}

		/* Child exit */
		exit(0);
	}

	/* connect to server socket created by our child subtask */
	sleep(2); /* wait for setup */
	if(connect(soc, (struct sockaddr *) &sock, sizeof(sock)) <0)
	{
		perror("parent: connect()");
		exit(-1);
	}

	/* send a message typed from interactived user to child subtask */
	while(1)
	{
		sleep(1); /* response time */
		printf("\nEnter a message: ");
		fflush(stdout);
		fgets(buffer, sizeof(buffer), stdin);

		if(send(soc, buffer, strlen(buffer)+1, 0) < 0)
		{
			perror("parent: send()");
			exit(-1);
		}
		if(strncmp(buffer, "EXIT", 4) == 0) /* exit request */
		{
			close(soc);
			break;
		}
	}
	/* await Child exit. */
	waitpid(pid);
	return;
}
