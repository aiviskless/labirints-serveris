#include "libs.h"
#include "consts.h"

// PARAMS
const int PORT = 8887;

// GLOBALS
int client_socket;

void process_join_request();

void *client_handler(void *client_socket) {
  int recv_size;
  int socket = *(int*)client_socket;
  char packet[256];
  char packet_type;

  // send map to client 
  // send(socket, food_map, sizeof(food_map), 0);
  puts("Thread successfully started");

  while ((recv_size = recv(socket, packet, sizeof(packet), 0)) > 0) {
    // get packet type
    packet_type = packet[0];
    // process packets
    switch (packet_type) {
      case JOIN_GAME:
        puts("Received JOIN_GAME request");
        process_join_request();
      case MOVE:
      case LOBBY_INFO:
      case GAME_IN_PROGRESS:
      case USERNAME_TAKEN:
      case GAME_START:
      case MAP_ROW:
      case GAME_UPDATE:
      case PLAYER_DEAD:
      case GAME_END:
      default:
        printf("Packet does not match any format: %s\n", packet);   
    }
  }

  if (recv_size == 0) {
    puts("Lost connection");
  } else if (recv_size < 0) {
    puts("Error while receiving data");
  }

  // free the socket
  close(socket);
  free(client_socket);
}

int main()
{
  int server_socket, client_socket, *new_socket;
  struct sockaddr_in, server;
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

  // client_socket = accept(server_socket, NULL, NULL);

  // process connections
  while (client_socket = accept(server_socket, NULL, NULL)) {
    puts("We have a connection");

    new_socket = malloc(1);
		*new_socket = client_socket;

    if (pthread_create(&thread, NULL, client_handler, (void*) new_socket) < 0) {
      puts("Error while creating thread");
      return 1;
    }
    puts("Thread created");
    
  }

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
