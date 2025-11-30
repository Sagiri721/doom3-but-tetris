/**
 * @file        input.c
 * @brief       Game input handling core functions
 */
#include "input.h"
#include "../queue/queue.h"
#include "tetris.h"

#include <stdlib.h>
#include <assert.h>

void register_input(int action, tetris_board* game) {
    
    assert(game != NULL);

    // To avoid queue overflow, we do not consume inputs if the game is over
    if (game->game_over) return;

    if(!enqueue(&game->input_queue, action)) {
        // Handle this maybe?
    }
}

void process_input(tetris_board* game) {
    if (game->input_provider && game->input_provider->process_fn) {
        game->input_provider->process_fn(game);
    }
}