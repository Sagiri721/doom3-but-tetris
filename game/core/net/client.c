/**
 * @file        client.c
 * @brief       UDP client to communicate with game server
 */
#include "client.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>

buffer_t global_buffer = {
    .data = NULL,
    .size = 0,
    .capacity = MAX_PACKET_SIZE,
};

int client_init(udp_client* client, const char* host, int port){

    client->sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (client->sockfd < 0) {
        fprintf(stderr, "Failed to create UDP socket");
        return -1;
    }

    memset(&client->server_addr, 0, sizeof(client->server_addr));
    client->server_addr.sin_family = AF_INET;
    client->server_addr.sin_port = htons(port);
    client->server_addr.sin_addr.s_addr = inet_addr(host);

    // Connect
    if (connect(client->sockfd, (struct sockaddr*) &client->server_addr, sizeof(client->server_addr)) < 0) {
        fprintf(stderr, "Failed to connect to UDP socket");
        return -1;
    }

    // Make socket non-blocking
    fcntl(client->sockfd, F_SETFL, O_NONBLOCK);

    // Fill global buffer
    global_buffer.data = malloc(MAX_PACKET_SIZE);
    assert(global_buffer.data);

    return 0;
}

// ts better never be called before init or I'm FINISHED
int client_send(udp_client* client, packet_types_t* p)
{
    // reset buffer and serialize
    global_buffer.size = 0;
    serialize_packet(&global_buffer, p);
    
    return send(client->sockfd, global_buffer.data, global_buffer.size, 0);
}

int client_receive(udp_client* client, void* buffer, int buffer_size)
{
    return recv(client->sockfd, buffer, buffer_size, 0);
}

void client_destroy(udp_client* client)
{
    close(client->sockfd);
}