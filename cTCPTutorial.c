#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

//simple socket program to print out any data received by tcp

int main(int argc, char const *argv[]) {
  //create socket
  int network_socket;
  //domain, type of socket, specifies protocol
  network_socket = socket(AF_INET, SOCK_STREAM, 0);

  //connect to another socket netinet has a structure for address and remote port

  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET; //sets type of address
  server_address.sin_port = htons(9002); //sets port
  server_address.sin_addr.s_addr = INADDR_ANY; //connects to localhost

  //socket, server address, size of the address, returns a success int
  int connection_status = connect(network_socket, (struct sockaddr *) &server_address, sizeof(server_address));

  //check for error with connection
  if (connection_status == -1) {
    printf("There was an error making a connection to the remote socket \n\n", );
  }

  //call receive function to receive data
  char server_response[256]; //just made a variable to store the data we got back from the receive
  recv(network_socket, &server_response, sizeof(server_response), 0);

  //printout the data that we just got
  printf("The server sent the data: %s\n", server_response);

  //closing socket
  close(network_socket);

  return 0;
}
