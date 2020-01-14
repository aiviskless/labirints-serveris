#include "libs.h"
#include "consts.h"

bool username_exists(char* username, Game *game) {
  if (game->players->head == NULL) {
    return false;
  }
  Player *p_player = game->players->head;
  while (strcmp(p_player->name,username) != 0) {
    if (p_player->next == NULL) {
      return false;
    } else {
      p_player = p_player->next;
    }
  }
  return true;
}

PlayerList *create_playerlist() {
  PlayerList *list = malloc(sizeof(PlayerList));
  list->head = NULL;
  list->tail = NULL;
  list->count = 0;

  return list;
}

Player *create_player(Game *game, char* username, int client_socket) {
  Player *new_player = malloc(sizeof(Player));

  new_player->name = username;
  new_player->socket = &client_socket;

  if (game->players->head == NULL) {
    game->players->head = new_player;
    game->players->tail = new_player;
  } else {
    game->players->tail->next = new_player;
    game->players->tail = new_player;
  }

  return new_player;
}