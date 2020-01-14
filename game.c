#include "game.h"
#include "helpers.c"

Game* game;


void start_game(server_socket, client_socket) {
  pthread_t thread;
  int *new_socket;
  int connections;

  // generate game structure
  game = malloc(sizeof(Game));

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
      ++connections;

      // if (connections == 8) {
      //   run_game_engine();
      // }
      
    }

    // run_game_engine();

    // if condition for stopping the server {
    //  break;
    // }
  }
}


void *client_processor(void *client_socket) {
  int recv_size;
  int socket = *(int*)client_socket;
  char packet[RESPONSE_LENGTH];
  Player *player;

  puts("Thread successfully started");

  // process packets
  while ((recv_size = recv(socket, packet, sizeof(packet), 0)) > 0) {
    switch (packet[0]) {
      case JOIN_GAME: {
        puts("Received JOIN_GAME request");
        player = process_join_request(socket, packet);
        break;
      }
      case MOVE: {
        puts("Received MOVE request");
        process_move_request(player, packet);
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


Player * process_join_request(int client_socket, char* packet) {
  Player *player = NULL;
  char* username = ++packet;
  // write(client_socket, packet, sizeof(packet));

  if (game->state == INGAME) {
    puts("Game in progress");
    write(client_socket, "3", sizeof("3"));
  } else if (username_exists(username, game)) {
    printf("Username taken: %s", username);
    write(client_socket, "4", sizeof("4"));
  } else {
    printf("Creating new player... %s", username);
    player = create_player(game, username, client_socket);
  }
  
  return player;
}


void process_move_request(Player* player, char* packet) {
  // do_things.exe
}

