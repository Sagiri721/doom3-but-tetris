/**
 * @file        game.h
 * @brief       Game loop implementation
 */

#include "game.h"

#include "gfx/render.h"
#include "core/tetris.h"

#define ROWS 10
#define COLS 20

tetris_board game;

void setup_game() {
    tetris_init(&game, ROWS, COLS);
}

void cleanup_game() {
    tetris_destroy(&game);
}

void update_game() {
    
    // Render the game
    render_begin();
    render_game(&game);
    render_end();
}