/**
 * @file        tetris.c
 * @brief       Implementation of the tetris game
 */

#include "tetris.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

position TETROMINOS[NUM_TETROMINOS][NUM_ORIENTATIONS][TETRIS] = {
  // I
  {{{1, 0}, {1, 1}, {1, 2}, {1, 3}},
   {{0, 2}, {1, 2}, {2, 2}, {3, 2}},
   {{3, 0}, {3, 1}, {3, 2}, {3, 3}},
   {{0, 1}, {1, 1}, {2, 1}, {3, 1}}},
  // J
  {{{0, 0}, {1, 0}, {1, 1}, {1, 2}},
   {{0, 1}, {0, 2}, {1, 1}, {2, 1}},
   {{1, 0}, {1, 1}, {1, 2}, {2, 2}},
   {{0, 1}, {1, 1}, {2, 0}, {2, 1}}},
  // L
  {{{0, 2}, {1, 0}, {1, 1}, {1, 2}},
   {{0, 1}, {1, 1}, {2, 1}, {2, 2}},
   {{1, 0}, {1, 1}, {1, 2}, {2, 0}},
   {{0, 0}, {0, 1}, {1, 1}, {2, 1}}},
  // O
  {{{0, 1}, {0, 2}, {1, 1}, {1, 2}},
   {{0, 1}, {0, 2}, {1, 1}, {1, 2}},
   {{0, 1}, {0, 2}, {1, 1}, {1, 2}},
   {{0, 1}, {0, 2}, {1, 1}, {1, 2}}},
  // S
  {{{0, 1}, {0, 2}, {1, 0}, {1, 1}},
   {{0, 1}, {1, 1}, {1, 2}, {2, 2}},
   {{1, 1}, {1, 2}, {2, 0}, {2, 1}},
   {{0, 0}, {1, 0}, {1, 1}, {2, 1}}},
  // T
  {{{0, 1}, {1, 0}, {1, 1}, {1, 2}},
   {{0, 1}, {1, 1}, {1, 2}, {2, 1}},
   {{1, 0}, {1, 1}, {1, 2}, {2, 1}},
   {{0, 1}, {1, 0}, {1, 1}, {2, 1}}},
  // Z
  {{{0, 0}, {0, 1}, {1, 1}, {1, 2}},
   {{0, 2}, {1, 1}, {1, 2}, {2, 1}},
   {{1, 0}, {1, 1}, {2, 1}, {2, 2}},
   {{0, 1}, {1, 0}, {1, 1}, {2, 0}}},
};

/**
 * Retrieve next pieces with RNG
 */
tetromino retrieve_next_piece() {
    int r = rand() % 7;
    return (tetromino) {
        .rot = 0,
        .type = r
    };
}

/**
 * Place a tetromino at the top of the screen
 */
void place_piece_at_top(tetris_board* game, tetromino* piece) {
    piece->position.x = floor(game->rows / 2.0);
    piece->position.y = game->cols;
}

/**
 * Clear the tetris board
 */
void clear_board(tetris_board* game) {

    assert(game->board != NULL);

    for (int x = 0; x < game->rows; x++) {
        for (int y = 0; y < game->cols; y++) {
            game->board[x * game->cols + y] = rand() % 7;
        }
    }
}

void tetris_init(tetris_board* game, int rows, int cols) {
    game->rows = rows;
    game->cols = cols;

    game->points = 0;
    game->level = 1;

    game->has_hold = 0;
    game->current = retrieve_next_piece();
    game->next = retrieve_next_piece();

    place_piece_at_top(game, &game->current);

    // Initialize field to empty
    game->board = malloc(rows * cols * sizeof(char*));
    if (game->board == NULL) {
        fprintf(stderr, "Failed to allocate memory for tetris board\n");
        exit(EXIT_FAILURE);
    }

    clear_board(game);
}

char index_cell(tetris_board* game, int x, int y) {
    
    assert(game->board != NULL);
    assert(x >= 0 && x < game->rows);
    assert(y >= 0 && y < game->cols);

    return game->board[x * game->cols + y];
}

void tetris_destroy(tetris_board *game) {

    assert(game->board != NULL);
    free(game->board);
}