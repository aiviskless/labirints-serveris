#ifndef MAZE_SERVER_GAME_H
#define MAZE_SERVER_GAME_H

#include "libs.h"
#include "consts.h"

typedef struct Player{
  char* name;
  int socket;
  int pos_x;
  int pos_y;
  struct Player *next;
  struct Player *prev;
} Player;

typedef struct PlayerList{
  Player* head;
  Player* tail;
  int count;
} PlayerList;

typedef struct {
  int state;
  PlayerList* players;
} Game;

void start_game(int, int);

PlayerList *create_playerlist();

void *client_processor(void *);

Player * process_join_request(int, char*);

void process_move_request(Player*, char*);

void update_lobby_info();

void start_round();

#endif
