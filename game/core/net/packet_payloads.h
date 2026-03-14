/**
 * @file        packet_payloads.h
 * @brief       Definr payloads for each packet
 */
#ifndef PACKET_PAYLOADS_H
#define PACKET_PAYLOADS_H

#include <time.h>

// Packet payloads
typedef struct connect {
#define CONNECT_FIELDS(_F, ...)     \
    _F(username, __VA_ARGS__)       \
    _F(connect_time, __VA_ARGS__)
    const char* username;
    time_t connect_time;
} connect_t;

typedef struct disconnect {
#define DISCONNECT_FIELDS(_F, ...)  \
    _F(username, __VA_ARGS__)       \
    _F(disconnect_time, __VA_ARGS__)
    const char* username;
    time_t disconnect_time; 
} disconnect_t;

typedef struct send_input {
#define SEND_INPUT_FIELDS(_F, ...)  \
    _F(username, __VA_ARGS__)       \
    _F(input_time, __VA_ARGS__)     \
    _F(input, __VA_ARGS__)          
    const char* username;
    time_t input_time; 
    input_event_type input; 
} send_input_t;

typedef struct none {
#define NONE_FIELDS(_F, ...)
} none_t; /* game struct as bytes */

#endif