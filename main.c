#include "libs.h"
#include "consts.h"

// PARAMS
const int PORT = 8887;

// GLOBALS
int client_socket;

void process_join_request();

int main()
{
  // create socket
  int server_socket;
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket < 0)
  {
    printf("Error while creating socket\n");
    return 1;
  }
  printf("Socket created\n");

  // set server address
  struct sockaddr_in server;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);

  // bind socket to the specified IP and port
  if (bind(server_socket, (struct sockaddr *)&server, sizeof(server)) < 0) {
    printf("Error while binding\n");
    return 1;
  }
  printf("Socket binded\n");

  // listen
  listen(server_socket, 30);
  client_socket = accept(server_socket, NULL, NULL);

  process_join_request();

  // close
  close(server_socket);

  return 0;
}

void process_join_request() {
  char response[RESPONSE_LENGTH];
  char *responseToClient = "2Test";

  // read JOIN_GAME
  if (read(client_socket, response, RESPONSE_LENGTH) < 0) {
    puts("Read from server failed");
  } else {
    switch (response[0]) {
      case JOIN_GAME:
        write(client_socket, responseToClient, sizeof(responseToClient));
        break;
      default:
        break;
    }
  }
}