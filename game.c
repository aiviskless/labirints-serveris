#import "game.h"

Game* game;


void start_game(server_socket, client_socket) {
  pthread_t thread;
  int *new_socket;
  int connections;

  // generate game structure
  game = malloc(sizeof(game));

  game->state = LOBBY;
  game->players = create_playerlist();

  // support for multiple rounds
  while (1) {
    connections = 0;

    // process multiple connections
    while ((client_socket = accept(server_socket, NULL, NULL))) {
      puts("We have a connection");

      new_socket = malloc(1);
      *new_socket = client_socket;

      if (pthread_create(&thread, NULL, client_processor, (void*) new_socket) < 0) {
        puts("Error while creating thread");
        exit(1);
      }
      puts("Thread created");
      connections = connections + 1;

      if (game->state == INGAME) {
        break;
      }
      
    }

    // run_game_engine();

    // if condition for stopping the server {
    //  break;
    // }
  }
}


PlayerList *create_playerlist() {
  PlayerList *list = malloc(sizeof(PlayerList));
  list->head = NULL;
  list->tail = NULL;
  list->count = 0;

  return list;
}


void *client_processor(void *client_socket) {
  int recv_size;
  int socket = *(int*)client_socket;
  char packet[RESPONSE_LENGTH];

  // send map to client 
  // send(socket, food_map, sizeof(food_map), 0);
  puts("Thread successfully started");

  // process packets
  while ((recv_size = recv(socket, packet, sizeof(packet), 0)) > 0) {
    switch (packet[0]) {
      case JOIN_GAME: {
        puts("Received JOIN_GAME request");
        process_join_request(socket, packet);
        break;
      }
      case MOVE: {
        puts("Received MOVE request");
        process_move_request();
        break;
      }
      default: {
        printf("Packet does not match any format: %s", packet); 
        break;
      }  
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

  return 0;
}


void process_join_request(int client_socket, char* packet) {
  write(client_socket, packet, sizeof(packet));
}


void process_move_request() {
  // do_things.exe
}
