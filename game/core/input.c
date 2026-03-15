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

// Verify if this input can be enqueued
// Unfortunate layer to make sure both client and servers are aware of input rules without repeating code in 
// input providers (which would be even worse)
int validate_input(input_validator_t* v, input_event_type action, uint32_t now) {
    switch (action) {
        case IE_MOVE_LEFT:
        case IE_MOVE_RIGHT:
            if ((now - v->last_move_time) < MOVE_COOLDOWN_MS) return 0;
            v->last_move_time = now;
            return 1;
        case IE_DROP:
            if ((now - v->last_drop_time) < DROP_COOLDOWN_MS) return 0;
            v->last_drop_time = now;
            return 1;
        default:
            return 1;
    }
}

void register_input(input_event_type action, tetris_board* game) {
 
    if (!validate_input(&game->validator, action, now_ms())) return;

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