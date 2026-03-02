/**
 * @file        client.h
 * @brief       UDP client to communicate with game server
 */
#ifndef CLIENT_H
#define CLIENT_H

#include <netinet/in.h>

#include "packets.h"
#include "buffer.h"

// I was gonna make this __thread but it made the game crash
extern buffer_t global_buffer;

typedef struct {
    int sockfd;
    struct sockaddr_in server_addr;
} udp_client;

// Initialize client and connect to server
int client_init(udp_client* client, const char* ip, int port);

// Send raw data
int client_send(udp_client* client, packet_types_t* data);

// Receive data (non-blocking)
int client_receive(udp_client* client, void* buffer, int buffer_size);

// Cleanup
void client_destroy(udp_client* client);

#endif