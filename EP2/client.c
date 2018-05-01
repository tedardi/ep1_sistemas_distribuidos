#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/sockets.h>
#include <netinet/in.h>
#include <netdb.h>

#define PORT 9876
#define NUM 50

main(int argc, char *argv[])
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
    gettimeofday(&bef, NULL);

    // random integer from 1 through 5
    random = rand() / (double) RAND_MAX;
    jj = (int) ((double) (5.0) * random) + 1;
    if(jj == 6)
      jj = 5;
    sprintf(buf, "/tmp/sample.txt %d", jj);

  }



}
