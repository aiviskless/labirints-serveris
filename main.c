#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

// PARAMS
const int PORT = 8887;

// GLOBALS

int main()
{
  // create socket
  int server_socket;
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket < 0)
  {
    printf("Error while creating socket");
    return 1;
  }
  printf("Socket created");

  // set server address
  struct sockaddr_in server;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);

  // bind socket to the specified IP and port
  if (bind(server_socket, (struct sockaddr *)&server, sizeof(server)) < 0) {
    printf("Error while binding");
    return 1;
  }
  printf("Socket binded");

  // listen
  listen(server_socket, 30);

  int client_socket;
  client_socket = accept(server_socket, NULL, NULL);

  // close
  close(server_socket);

  return 0;
}
