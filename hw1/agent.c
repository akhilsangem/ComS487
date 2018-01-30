#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <netinet/in.h>
#include <unistd.h>
void *heartBeat(void *arg){
  typedef struct BEACON
  {
    int ID; // randomly generated during startup
    int StartUpTime; // the time when the client starts
    char IP[4]; // the IP address of this client
    int CmdPort; // the client listens to this port for manager cmd
  } beacon;

  srand((unsigned) time(NULL));
  beacon b;
  b.ID = rand() % 100;
  b.StartUpTime = (unsigned long)time(NULL);
  b.IP[0] = '0';
  b.IP[1] = '0';
  b.IP[2] = '0';
  b.IP[3] = '0';
  b.CmdPort = 9003;
  while (1) {
      usleep(1);
      printf("sending heartBeat to manager\n");
  }

  return NULL;
}

int main(int argc, char const *argv[], char * envp[]) {
  pthread_t beaconHeart;
  pthread_create(&beaconHeart,NULL,heartBeat,NULL);
  while (1) {
    printf("listening to tcp\n");
  }
  return 0;
}
