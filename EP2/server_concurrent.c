#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>

#define PORT 9876

// Server: subtask per query
int main(void)
{
  // Structure for socket()
  struct sockaddr_in sock;
  // Structure len for aceept()
  int socklen = sizeof(sock);

  // Sample.txt file pointer
  FILE *fp;

  // Accept file descriptor
  int acc;

  // Client file descriptor
  int cli;

  // Requested line number
  int line;

  // Setsockopt parameter
  int opt = 1;

  // Socket receive buffer
  char buf[80];

  // Text file from client
  char file[32];

  // Create a (server) socket to accept a client
  acc = socket(AF_INET, SOCK_STREAM, 0);

  // bind socket to port
  sock.sin_family = AF_INET;
  sock.sin_port = htons(PORT);
  sock.sin_addr.s_addr = htonl(INADDR_ANY);
  setsockopt(acc, SOL_SOCKET, SO_REUSEADDR, (char *) &opt, sizeof(opt));
  bind(acc, (struct sockaddr *) &sock, socklen);

  while(1)
  {
    // await connection from remote client
    cli = accept(acc, (struct sockaddr *) &sock, &socklen);

    // get a random line of data requested by the client
    if(fork() == 0)
    {
        recv(cli, buf, sizeof(buf), 0);

        // Open the requested file
        sscanf(buf, "%s %d", file, &line);
        fp = fopen(file, "r");
        while(line--)
          fgets(buf, sizeof(buf), fp);
        fclose(fp);

        buf[strlen(buf)-1] = '\0';
        send(cli, buf, strlen(buf)+1, 0);
        close(cli);
        exit(0);
    }
    close(cli);
  }
}
