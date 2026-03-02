/**
 * @file        input.c    
 * @brief       Implementation of input registration and processing for Tetris game.
 */

#include "input.h"
#include "queue/queue.h"
#include "tetris.h"
#include "net/packets.h"

#include <stdlib.h>
#include <time.h>

void register_input(input_event_type action, tetris_board* game) {

    if(!enqueue(&game->input_queue, action)) {
        // Handle this maybe?
    }

    // Dup input into socket if provided
    if (game->server) {
        
        // client_send(game->server, &(packet_types_t) {
        //     .none = {},
        //     .type = PACKET_TYPE_NONE
        // });
    }
}

// Dispatch input processing to the appropriate input provider
void pump_input(input_provider* provider, tetris_board* game) {
    if (provider && provider->process_fn) {
        provider->process_fn(game);
    }
}