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

#include <stdlib.h>

#include "packets.h"

#define PORT 5000

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

    printf("Server listening on port %d\n", PORT);

    while (1)
    {
        uint8_t buffer[MAX_PACKET_SIZE];

        socklen_t len = sizeof(cliaddr);
        size_t recvlen = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&cliaddr, &len);

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
               inet_ntoa(cliaddr.sin_addr),
               ntohs(cliaddr.sin_port));

        reader_t reader = {
            .data = buffer,
            .size = recvlen,
            .pos  = 0
        };

        packet_types_t packet = {};

        deserialize_packet(&reader, &packet);

        switch (packet.type)
        {
            case PACKET_TYPE_CONNECT:
                printf("CONNECT: username=%s\n",
                       packet.connect.username);

                free(packet.connect.username);                    
                break;

            case PACKET_TYPE_DISCONNECT:
                printf("DISCONNECT: username=%s\n",
                       packet.disconnect.username);

                free(packet.disconnect.username);                    

                break;

            case PACKET_TYPE_NONE:
            default:
                printf("NONE or unknown packet\n");
                break;
        }

        printf("\n");
    }

    return 0;
}