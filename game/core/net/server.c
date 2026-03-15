/**
 * @file        server.c
 * @brief       UDP game server
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "lib/tinycthread.h"

#include "lobby.h"
#include "net/packets.h"

#define PORT 5000

thrd_t server_thread;

struct server_data {
    struct sockaddr_in servaddr, cliaddr;
    int sockfd;
};

void print_board(const tetris_board* game) {
    if (!game || !game->board) {
        printf("[board not initialized]\n");
        return;
    }

    printf("\033[2J\033[H");  

    printf("=== %s | lvl:%d pts:%d lines:%d %s ===\n",
        game->name ? game->name : "?",
        game->level,
        game->points,
        game->stats.lines_cleared,
        game->game_over ? "GAME OVER" : "");

    // Top border
    printf("+");
    for (size_t x = 0; x < game->cols; x++) printf("--");
    printf("+\n");

    for (size_t y = 0; y < game->rows; y++) {
        printf("|");
        for (size_t x = 0; x < game->cols; x++) {
            char cell = index_cell(game, x, y);

            // Check if current piece occupies this cell
            char is_piece = 0;
            for (int p = 0; p < TETRIS; p++) {
                position c = TETROMINOS[game->current.type][game->current.rot][p];
                if ((int)(c.x + game->current.pos.x) == (int)x &&
                    (int)(c.y + game->current.pos.y) == (int)y) {
                    is_piece = 1;
                    break;
                }
            }

            if (is_piece)        printf("[]");
            else if (cell == 0)  printf(" .");
            else                 printf("##");
        }
        printf("|\n");
    }

    // Bottom border
    printf("+");
    for (size_t x = 0; x < game->cols; x++) printf("--");
    printf("+\n");

    // Next piece preview
    printf("next: ");
    static const char* names[] = {"I","J","L","O","S","T","Z"};
    if (game->next.type < NUM_TETROMINOS)
        printf("%s", names[game->next.type]);
    printf("\n\n");
}

int server_loop(void* args) {
    
    struct server_data* data = (struct server_data*) args;
    assert(data);

    printf("Server listening on port %d\n", PORT);

    // The lobby this thread is responsible for
    lobby_t lobby = {0};

    while (1)
    {
        uint8_t buffer[MAX_PACKET_SIZE];

        socklen_t len = sizeof(data->cliaddr);
        size_t recvlen = recvfrom(data->sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&data->cliaddr, &len);

        if (recvlen < 0) {
            perror("recvfrom");
            continue;
        }

        printf("Raw bytes:\n");

        for (size_t i = 0; i < recvlen; i++) {
            printf("%02X ", buffer[i]);
        }
        printf("\n");

        printf("Packet received (%ld bytes)\n", recvlen);
        printf("  From: %s:%d\n",
               inet_ntoa(data->cliaddr.sin_addr),
               ntohs(data->cliaddr.sin_port));

        reader_t reader = {
            .data = buffer,
            .size = recvlen,
            .pos  = 0
        };

        packet_types_t packet = {};

        deserialize_packet(&reader, &packet);
        //        printf("%d\n", packet.type);
        
        switch (packet.type)
        {
            case PACKET_TYPE_CONNECT:
                printf("CONNECT: username=%s time=%ld\n",   
                packet.connect.username, packet.connect.connect_time);

                spawn_player(&lobby, packet.connect.username, 0);

                free(packet.connect.username);
                break;

            case PACKET_TYPE_DISCONNECT:
                printf("DISCONNECT: username=%s time=%ld\n",
                    packet.disconnect.username, packet.disconnect.disconnect_time);

                remove_player(&lobby, packet.disconnect.username);

                free(packet.disconnect.username);
                break;

            case PACKET_TYPE_SEND_INPUT: {
                printf("INPUT: username=%s input=%d\n",
                    packet.send_input.username, packet.send_input.input);

                lobby_player_t* p = get_player(&lobby, packet.send_input.username);
                enqueue(&p->game.input_queue, packet.send_input.input);
                tetris_process_input_queue(&p->game);

                free(packet.send_input.username);
                break;
            }
            default:
                printf("NONE or unknown packet\n");
                break;
        }

        if (lobby.players[1].game.name)
            print_board(&lobby.players[1].game);

        printf("\n");
    }

    return 0;
}

int server(void)
{
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    struct server_data svdata = (struct server_data) {
        .cliaddr = cliaddr,
        .servaddr = servaddr,
        .sockfd = sockfd
    };

    thrd_create(&server_thread, server_loop, &svdata);
    thrd_join(server_thread, NULL);

    return 0;
}