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

int main(void)
{
  // SLAVE: activated by a UDP datagram to port 3579

  struct sockaddr_in sock;
  int len = sizeof(sock);
  struct hostent *hp;
  int port;
  int fd;
  char host[16];
  char buf[80];

  // Get host and port from MASTER
  read(0, buf, sizeof(buf));
  sscanf(buf, "%s %d", host, &port);

  //Initialize a UDP socket struct
  fd = socket(AF_INET, SOCK_DGRAM, 0);
  memset((char *) &sock, 0, sizeof(sock));
  // MASTER
  hp = gethostbyname(host);
  memcpy(&sock.sin_addr, hp->h_addr, hp->h_length);
  sock.sin_port = htons(port);
  sock.sin_family = AF_INET;

  //Register with MASTER
  gethostname(buf, sizeof(buf));
  sendto(fd, buf, strlen(buf) + 1, 0, (struct sockaddr *) &sock, sizeof(sock));

  // Further processing
  printf("Sou um escravo em execucao!!\n");

  close(fd);
  return 0;
}
