
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <strings.h>

void *heartBeat(void *arg){
  typedef struct BEACON
  {
    int ID; // randomly generated during startup
    int CmdPort; // the client listens to this port for manager cmd
    int StartUpTime; // the time when the client starts
    char IP[4]; // the IP address of this client

  } beacon;

  srand((unsigned) time(NULL));
  beacon b;
  b.ID = rand() % 100;
  b.CmdPort = 9002;
  b.StartUpTime = (int)time(NULL);
  b.IP[0] = '0';
  b.IP[1] = '0';
  b.IP[2] = '0';
  b.IP[3] = '0';

  printf("ID is %d\n", b.ID);

  int sock, length, fromlen, n;
  struct sockaddr_in server;
  struct sockaddr_in from;
  //char buf[1024];
  int size = sizeof(b);
  char buf[size];
  sock=socket(AF_INET, SOCK_DGRAM, 0);
  if (sock < 0)
  {
    printf("Opening socket\n");
  }
  length = sizeof(server);
  bzero(&server,length);
  server.sin_family=AF_INET;
  server.sin_addr.s_addr=INADDR_ANY;
  server.sin_port=htons(9002);

  fromlen = sizeof(struct sockaddr_in);

  memset(buf, 0x00, size);
  memcpy(buf, &b, size);

  while(1){
    usleep(600000);

    //n = sendto(sock,"Got your message\n",17,0, (struct sockaddr *)&from,fromlen);
    n = sendto(sock,buf,sizeof(buf),0, (struct sockaddr *)&server,fromlen);
    while (n < 0)
    {
      printf("sendto failed\n");
      n = sendto(sock,buf,17,0, (struct sockaddr *)&server,fromlen);
    }

  }
  close(sock);
  return NULL;
}

int main(int argc, char const *argv[], char * envp[]) {
  pthread_t beaconHeart;
  pthread_create(&beaconHeart,NULL,heartBeat,NULL);
  while (1) {
    usleep(600000);
    printf("listening to tcp\n");
  }
  pthread_join(beaconHeart,NULL);
  return 0;
}
