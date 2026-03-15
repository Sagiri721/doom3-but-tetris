/**
 * @file        lobby.c
 * @brief       Game lobby, represents the state of a multiplayer game
 */
#include "lobby.h"
#include <string.h>

static int find_slot(lobby_t* lobby, const char* player) {
    for (int i = 0; i < 2; i++)
        if (lobby->players[i].game.name &&
            strcmp(lobby->players[i].game.name, player) == 0)
            return i;
    
    return -1;
}

static int empty_slot(lobby_t* lobby) {
    for (int i = 0; i < 2; i++)
        if (!lobby->players[i].game.name)
            return i;
    
    return -1;
}

int spawn_player(lobby_t* lobby, const char* player, unsigned int seed) {
    int slot = empty_slot(lobby);
    if (slot < 0) return 1;

    lobby_player_t* p = &lobby->players[slot];
    
    tetris_init(&p->game, ROWS, COLS, seed, strdup(player));
    p->last_move_time = 0;
    p->last_drop_time = 0;

    return 0;
}

lobby_player_t* get_player(lobby_t* lobby, const char* player) {

    int slot = find_slot(lobby, player);
    return slot >= 0 ? &lobby->players[slot] : NULL;
}

void remove_player(lobby_t* lobby, const char* player) {
    
    int slot = find_slot(lobby, player);
    if (slot < 0) return;
    
    tetris_destroy(&lobby->players[slot].game);
    memset(&lobby->players[slot], 0, sizeof(lobby_player_t));
}

int lobby_full(lobby_t* lobby) {
    
    return empty_slot(lobby) < 0;
}