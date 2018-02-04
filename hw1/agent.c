
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
    usleep(1);

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
  //pthread_t beaconHeart;
  //pthread_create(&beaconHeart,NULL,heartBeat,NULL);

  while (1) {
    usleep(1);
    printf("doing tcp\n");

    int network_socket;
    //domain, type of socket, specifies protocol
    network_socket = socket(AF_INET, SOCK_STREAM, 0);

    //connect to another socket netinet has a structure for address and remote port

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET; //sets type of address
    server_address.sin_port = htons(9004); //sets port
    server_address.sin_addr.s_addr = INADDR_ANY; //connects to localhost

    //socket, server address, size of the address, returns a success int
    int connection_status = connect(network_socket, (struct sockaddr *) &server_address, sizeof(server_address));

    //check for error with connection
    if (connection_status == -1) {
      printf("There was an error making a connection to the remote socket \n\n");
    }

    //call receive function to receive data
    char server_response[256]; //just made a variable to store the data we got back from the receive
    recv(network_socket, &server_response, sizeof(server_response), 0);

    //printout the data that we just got
    printf("The server sent the data: %s\n", server_response);

    //closing socket
    close(network_socket);
  }

  //pthread_join(beaconHeart,NULL);
  return 0;
}
