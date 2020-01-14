#include "libs.h"
#include "consts.h"
#include "game.c"

// PARAMS
const int PORT = 8887;

int main()
{
  int server_socket, client_socket, *new_socket;
  struct sockaddr_in server;
  pthread_t thread;

  // create socket
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket < 0)
  {
    puts("Error while creating socket");
    return 1;
  }
  puts("Socket created");

  // set server address
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);

  // enables port reuse after program ends
  if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)) < 0) {
    perror("setsockopt(SO_REUSEADDR) failed");
  }

  // bind socket to the specified IP and port
  if (bind(server_socket, (struct sockaddr *)&server, sizeof(server)) < 0) {
    puts("Error while binding");
    return 1;
  }
  puts("Socket binded");

  // listen
  listen(server_socket, 30);

  // main server function
  start_game(server_socket, client_socket );

  // close
  close(server_socket);

  return 0;
}
