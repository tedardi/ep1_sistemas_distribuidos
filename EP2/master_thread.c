#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>


#define NUMTHREADS 4
#define PORT 9876

struct ThreadServer {
    char buf;
    int cli;
};

void* slave(int cli) {
    // Sample.txt file pointer
     FILE *fp;

    // Requested line number
    int line;

    // Text file from client
    char file[32];

    char buf[80];
 
    recv(cli, buf, sizeof(buf), 0);

    sscanf(buf, "%s %d", file, &line);
    fp = fopen(file, "r");
    while(line--)
      fgets(buf, sizeof(buf), fp);
    fclose(fp);

    buf[strlen(buf)-1] = '\0';
    send(cli, buf, strlen(buf)+1, 0);
    close(cli);
 }


int main(void) {

  struct ThreadServer tdata;
  
   // Structure for socket()
  struct sockaddr_in sock;
  // Structure len for aceept()
  int socklen = sizeof(sock);

  // Sample.txt file pointer
  FILE *fp;

  // Accept file descriptor
  int acc;

  // Client file descriptor
  //int cli;

  // Requested line number
  int line;

  // Setsockopt parameter
  int opt = 1;

  // Socket receive buffer
  char buf[80];

  // Text file from client
  char file[32];

  int cli;


  pthread_t thread[NUMTHREADS];

  // Create a (server) socket to accept a client
  acc = socket(AF_INET, SOCK_STREAM, 0);

  // bind socket to port
  sock.sin_family = AF_INET;
  sock.sin_port = htons(PORT);
  sock.sin_addr.s_addr = htonl(INADDR_ANY);
  setsockopt(acc, SOL_SOCKET, SO_REUSEADDR, (char *) &opt, sizeof(opt));
  bind(acc, (struct sockaddr *) &sock, socklen);

  int j = 0;
  
  for(int j= 1; j <1000; j++){
  
    listen(acc, 5);

    // get a random line pf data requested by the client
    cli = accept(acc, (struct sockaddr *) &sock, &socklen);
    
    pthread_create(&thread[j], NULL, slave, cli);
    pthread_join(thread[j], NULL);
    //slave(cli);
  } 

   
    return 0;
}