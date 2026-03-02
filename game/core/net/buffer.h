/**
 * @file        buffer.h
 * @brief       Network buffer
 */

#ifndef BUFFER_H
#define BUFFER_H

#include <stddef.h>
#include <stdint.h>

// Simple buffer
typedef struct {
    uint8_t* data;
    size_t capacity;
    size_t size;
} buffer_t;

typedef struct {
    uint8_t* data;
    size_t size;
    size_t pos;
} reader_t;

int write_bytes(buffer_t* b, const void* src, size_t n);
void write_u16(buffer_t* b, uint16_t v);
void write_u32(buffer_t* b, uint32_t v);

int read_u16(reader_t* r, uint16_t* out);
int read_u32(reader_t* r, uint32_t* out);
int read_bytes(reader_t* r, void* dest, size_t n);

#endif