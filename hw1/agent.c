
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <strings.h>
#include <sys/utsname.h>

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

  while(1)
  {
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

int main(int argc, char const *argv[], char * envp[])
{

  pthread_t beaconHeart;
  pthread_create(&beaconHeart,NULL,heartBeat,NULL);

  struct utsname unameData;
  uname(&unameData);

  char server_message[1];
  char server_response[256] = "Holy Shit you got to the server";

  //create server socket
  int server_socket;
  server_socket = socket(AF_INET, SOCK_STREAM, 0);

  //define server server_address
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET; //sets type of address
  server_address.sin_port = htons(9004); //sets port
  server_address.sin_addr.s_addr = INADDR_ANY; //connects to localhost

  //bind the socket to our specified ip and sin_port
  bind(server_socket, (struct sockaddr*)  &server_address, sizeof(server_address));

  int client_socket;

  while (1)
  {
    usleep(1);
    printf("Waiting for tcp request\n");
    listen(server_socket, 5);

    client_socket = accept(server_socket, NULL, NULL);
    //bind(client_socket, (struct sockaddr*)  &server_address, sizeof(server_address));
    //bzero(server_message,256);
    recv(client_socket, server_message, sizeof(server_message), 0);

    //printf("Enter 2 to receive OS data\n");
    if(server_message[0] == '2')
    {
      write(client_socket, unameData.sysname, 200);
    }

    write(client_socket, "You Have Connected to the C Server\n", 256);

    printf("%s\n", server_message);

    //close(client_socket);
  }
  //close(server_socket);
  pthread_join(beaconHeart,NULL);
  return 0;
}
