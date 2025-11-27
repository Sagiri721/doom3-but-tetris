/**
 * @file        render.h
 * @brief       Game rendering abstraction
 */

#ifndef RENDER_H
#define RENDER_H

#include "../core/tetris.h"

void render_begin();
void render_end();

void render_game(tetris_board* game);

#endif