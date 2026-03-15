/**
 * @file        lobby.h
 * @brief       Game lobby, represents the state of a multiplayer game
 */

#ifndef LOBBY_H
#define LOBBY_H

#include "../tetris.h"

#include <netinet/in.h>
#include <stdint.h>

typedef struct {
    tetris_board game;
    struct sockaddr_in addr;
    uint32_t last_move_time;
    uint32_t last_drop_time;
} lobby_player_t;

typedef struct {
    lobby_player_t players[2];
} lobby_t;

int spawn_player (lobby_t* lobby, const char* player, unsigned int seed);
lobby_player_t* get_player (lobby_t* lobby, const char* player);
void remove_player (lobby_t* lobby, const char* player);
int lobby_full (lobby_t* lobby);

#endif