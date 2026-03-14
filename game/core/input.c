/**
 * @file        input.c    
 * @brief       Implementation of input registration and processing for Tetris game.
 */

#include "input.h"
#include "queue/queue.h"
#include "tetris.h"
#include "net/client.h"
#include "utils.h"

#include <stdlib.h>
#include <string.h>

void register_input(input_event_type action, tetris_board* game) {
    
    // Use socket if provided
    if(!enqueue(&game->input_queue, action)) {
        // Handle this maybe?
    }

    // Relay input to server if available
    if (game->server) {
        client_send(game->server, &(packet_types_t) {
            .type = PACKET_TYPE_SEND_INPUT,
            .send_input = {
                .input = action,
                .username = strdup(game->name),
                .input_time = now_ms()
            },
        });
    }
}

// Dispatch input processing to the appropriate input provider
void pump_input(input_provider* provider, tetris_board* game) {
    if (provider && provider->process_fn) {
        provider->process_fn(game);
    }
}