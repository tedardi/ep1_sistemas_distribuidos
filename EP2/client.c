#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netdb.h>

#define PORT 9876
#define NUM 50

int main(int argc, char *argv[])
{

  // Timer value before/after
  struct timeval before, after;

  // Socket structure
  struct sockaddr_in sock;

  // Structure for IP address
  struct hostent *hp;

  double random;

  // Time remaining in IAT (Inter-Arrival Time: how much time separates packets)
  long timeleft;

  // Inter-arrival time seconds
  long secs=0L;

  // Inter-arrival time microseconds
  long usec=0L;

  // Subtask process ids
  int pids[NUM];

  // Setsockopt parameter
  int opt=1;

  // Socket file descriptor
  int fd;

  int ii, jj, kk = 0;

  char buf[80], *p;

  // Seed rand()
  srand((unsigned int) getpid());

  // Operands are remote HOST name and IAT (Inter-arrival Time)
  if(argc != 3)
  {
    printf("\n\tUsage: %s <HOST> <IAT>\n\n", argv[0]);
    exit(-1);
  }

  // uSeconds part of inter-arrival
  if((p=strchr(argv[2], '.')) != NULL)
  {
    // End whole number at decimal
    *p = '\0';
    // Bump pointer to start of mantissa
    p++;

    while(strlen(p) < 6)
    {
      // Pad out of 6 digits
      strcat(p, "0");
    }
    // Truncate to 6 digits max
    p[6] = '\0';
    usec = atol(p);
  }
  // Seconds part of IAT
  secs = atol(argv[2]);

  // LOOP: send and receive NUM packets
  for(ii=0; ii<NUM; ii++)
  {
    // get time before send
    gettimeofday(&before, NULL);

    // random integer from 1 through 5
    random = rand() / (double) RAND_MAX;
    jj = (int) ((double) (5.0) * random) + 1;
    if(jj == 6)
      jj = 5;
    sprintf(buf, "/tmp/sample.txt %d", jj);

    if((pids[kk++]=fork()) == 0)
    {
      // set up socket info for connect
      fd = socket(AF_INET, SOCK_STREAM, 0);
      memset((char *) &sock, 0, sizeof(sock));
      hp = gethostbyname(argv[1]);
      memcpy(&sock.sin_addr, hp->h_addr, hp->h_length);
      sock.sin_family = hp->h_addrtype;
      sock.sin_port = htons(PORT);
      setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *) &opt, sizeof(opt));

      // Connect to server
      connect(fd, (struct sockaddr *) &sock, sizeof(sock));
      send(fd, buf, strlen(buf) + 1, 0);

      // Limpa buffer
      buf[0] = 0;
      recv(fd, buf, sizeof(buf), 0);

      // Print out response to our query
      printf("\tLine %d: '%s' ", jj, buf);
      // Check the correct line
      p = strrchr(buf, ' ') + 2;
      if(jj != atoi(p))
        printf("*");
      printf("\n");
      close(fd);
      exit(0);
    }

    // Sleep for remainder of IAT
    gettimeofday(&after, NULL);
    after.tv_sec -= before.tv_sec;
    after.tv_usec -= before.tv_usec;
    if(after.tv_usec < 0L)
    {
      after.tv_usec += 1000000L;
      after.tv_sec -= 1;
    }
    before.tv_sec = secs;
    before.tv_usec = usec;
    before.tv_sec -= after.tv_sec;
    before.tv_usec -= after.tv_usec;

    if(before.tv_usec < 0L)
    {
        before.tv_usec += 1000000L;
        before.tv_sec -= 1;
    }
    timeleft = (before.tv_sec * 1000000L ) + before.tv_usec;
    if(timeleft < 0)
    {
      printf("\tERROR: A higher IAT value is requisred - exiting.\n");
      break;
    }
    usleep(timeleft);
  }

  for(ii=0; ii<kk; ii++)
  {
    waitpid(pids[ii], NULL, 0);
  }


  puts("Done.");

  return 0;
}
