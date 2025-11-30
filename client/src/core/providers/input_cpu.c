/**
 * @file        input.c
 * @brief       CPU input handling
 */

#include "input_cpu.h"
#include "../tetris.h"

void process_cpu_input(tetris_board *game) {

    // Ok i wont play anymore fine
    if (game->game_over) return;

    // drop drop drop
    register_input(IE_DROP, game);
}

void init_cpu_provider(input_provider *provider) {

    provider->type = INPUT_PROVIDER_CPU;
    provider->process_fn = process_cpu_input;
}