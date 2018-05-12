#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/signal.h>
#include <sys/wait.h>

#define PORT 9876
#define FALSE 0
#define TRUE 1
#define MAX 5

// Boolean to reset in control_C()
int Running = TRUE;

// Server: subtask pool
int main(void)
{

  // Flag for every subtask
  // 0=idle; 1=good; X=exit
  struct area
  {
      int flag[MAX];
  } area, *aptr;

  int flag = (IPC_CREAT | IPC_EXCL | 0660);
  int size = sizeof(struct area);
  key_t key = 0x5a5a5a5a;

  // Shared memory area id
  int shmid;

  // Control-c signal handler
  void control_C();

  // Structure for socket()
  struct sockaddr_in sock;
  // Structure len for aceept()
  int socklen = sizeof(sock);

  // Sample.txt file pointer
  FILE *fp;

  // Select waits for client
  fd_set fds;

  // Accept file descriptor
  int acc;

  // Client file descriptor
  int cli;

  // Requested line number
  int line;

  // Setsockopt parameter
  int opt = 1;

  // Internal subtask id
  int ident;

  int pids[MAX];

  // Socket receive buffer
  char buf[80];

  // Text file from client
  char file[32];

  // Initialize ^C handler
  signal(SIGINT, control_C);

  // Create a (server) socket to accept a client
  acc = socket(AF_INET, SOCK_STREAM, 0);

  // bind socket to port

  //REUSE IMPORTANTE PARA NÃO BLOQUEAR A PORTA
  int reuse = 1;
  sock.sin_family = AF_INET;
  sock.sin_port = htons(PORT);
  sock.sin_addr.s_addr = htonl(INADDR_ANY);
  if(setsockopt(acc, SOL_SOCKET, SO_REUSEADDR, (char *) &opt, sizeof(opt)) == -1)
    printf("Não conseguiu reutilizar a porta\n");
  if(bind(acc, (struct sockaddr *) &sock, socklen) == -1)
    printf("ERROR, cannot bind the port\n");
  //bind(acc, (struct sockaddr *) &sock, socklen);
  listen(acc, 5);

  // Get the shared memory area
  shmid = shmget(key, size, flag);

  // Attach subtask shared memory area
  aptr = (struct area *) shmat(shmid, 0, 0);

  // Pre-establish subtask pool
  for(ident=0; ident<MAX; ident++)
  {
    // Set flag idle
    aptr->flag[ident] = 0;

    // Subtask
    if((pids[ident] = fork()) == 0)
    {
        // Attach parent shared memory to array
        aptr = (struct area *) shmat(shmid, 0, 0);
        // Notice: shmid is still set correctly

        // nullify ^C handler
        signal(SIGINT, SIG_DFL);

        // Pool memory array for a non-zero flag
        while(1)
        {
          if(aptr->flag[ident] == 0)
          {
            // Release processor
            usleep(1);
            // Stay in pool loop
            continue;
          }
          else if(aptr->flag[ident] < 0)
          {
            // Subtask normal exist
            exit(0);
          }
          // Action flag must be positive
          else
          {
            // await connection from remote client
            cli = accept(acc, (struct sockaddr *) &sock, &socklen);
            recv(cli, buf, sizeof(buf), 0);

            // Open the requested file
            sscanf(buf, "%s %d", file, &line);
            fp = fopen(file, "r");
            while(line--)
              fgets(buf, sizeof(buf), fp);
            fclose(fp);

            // Send back a response
            buf[strlen(buf)-1] = '\0';
            send(cli, buf, strlen(buf)+1, 0);
            close(cli);

            // Set to available
            aptr->flag[ident] = 0;
          }
        }
    }
  }

  // Parent task passes incoming connections to a subtask
  // Set FALSE in control_C signal handler
  while(Running)
  {
    FD_ZERO(&fds);
    FD_SET(acc, &fds);
    // block until client is ready for connect
    if(select(acc+1, &fds, NULL, NULL, NULL) < 0)
      break;
    // Assign incoming query to first available subtask
    for(ident=0; ident<MAX; ident++)
    {
      if(aptr->flag[ident] == 0)
      {
        aptr->flag[ident] = 1;
        break;
      }
    }
  }

  // Wait for each subtask exit
  for(ident=0; ident<MAX; ident++)
  {
    aptr->flag[ident] = -1;
    waitpid(pids[ident], NULL, 0);
  }

  // Remove unused shared memory area and exit
  shmctl(shmid, IPC_RMID, (struct shmid_ds *) 0);
  return 0;
}

// ^C handler
void control_C()
{
  // Reset flag to allow exit
  Running = FALSE;
}
