#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PIXEL_SIZE 40 
#define TILE_SIZE_IN_PIXELS 8
#define TILE_SIZE (PIXEL_SIZE * TILE_SIZE_IN_PIXELS)

#define DRAW_SIZE 8

// gameplay params
#define PLAYER_SPEED 40

// arena map params
#define ARENA_WALLS "-|+"
#define ARENA_WIDTH_IN_TILES 36
#define ARENA_HEIGHT_IN_TILES 30
#define ARENA_HEIGHT (ARENA_HEIGHT_IN_TILES*TILE_SIZE)
#define ARENA_WIDTH (ARENA_WIDTH_IN_TILES*TILE_SIZE)

// PARAMS
const int PORT = 8887;

// GLOBALS
const char food_map[ARENA_HEIGHT_IN_TILES*ARENA_WIDTH_IN_TILES] = 
"+----------+-----------+-----------+"
"|.        .|.         .|.         .|"
"|          |           |           |"
"|     +----+----+  +---+-----+     |"
"|     |       ..| .|..       |     |"
"|  +  |       ..|. |..       |  +  |"
"|  |  |  +-------  -------+  |  |  |"
"|  |  |  |.              .|  |  |  |"
"|  |  |  |                |  |  |  |"
"|  |     |  +----------+  |     |  |"
"|  |     |   .        .   |     |  |"
"|  |  |  |                |  |  |  |"
"|  |  |  |  +  +-  -+  +  |  |  |  |"
"| .|  +--+  |  |.  .|  |  +--+  | .|"
"+--+   .    |          |    .   +--+"
"+--+    .   |          |   .    +--+"
"| .|  +--+  |  |.  .|  |  +--+  | .|"
"|  |  |  |  +  +-  -+  +  |  |  |  |"
"|  |  |  |                |  |  |  |"
"|  |     |   .        .   |     |  |"
"|  |     |  +----------+  |     |  |"
"|  |  |  |                |  |  |  |"
"|  |  |  |.              .|  |  |  |"
"|  |  |  +------+  +------+  |  |  |"
"|  +  |       ..|. |..       |  +  |"
"|     |       ..| .|..       |     |"
"|     +----+----+  +----+----+     |"
"|          |            |          |"
"|.        .|.          .|.        .|"
"+----------+------------+----------+"
;

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

  int client_socket;
  client_socket = accept(server_socket, NULL, NULL);



  // send map to client 
  send(client_socket, food_map, sizeof(food_map), 0);
  printf("Map sent to client\n");

  // close
  close(server_socket);

  return 0;
}
