#include "buffer.h"

#include <netinet/in.h>
#include <string.h>

// Write
int write_bytes(buffer_t* b, const void* src, size_t n) {

    if (b->size + n > b->capacity)
        return 1;

    memcpy(b->data + b->size, src, n);
    b->size += n;

    return 0;
}

void write_u16(buffer_t* b, uint16_t v) {
    v = htons(v);
    write_bytes(b, &v, sizeof(v));
}

void write_u32(buffer_t* b, uint32_t v) {
    v = htonl(v);
    write_bytes(b, &v, sizeof(v));
}

// Read
int read_u16(reader_t* r, uint16_t* out) {

    if (r->pos + sizeof(uint16_t) > r->size)
        return 1;

    uint16_t v;
    memcpy(&v, r->data + r->pos, sizeof(uint16_t));
    r->pos += sizeof(uint16_t);

    *out = ntohs(v);
    return 0;
}

int read_u32(reader_t* r, uint32_t* out) {

    if (r->pos + sizeof(uint32_t) > r->size)
        return 1;

    uint32_t v;
    memcpy(&v, r->data + r->pos, sizeof(uint32_t));
    r->pos += sizeof(uint32_t);

    *out = ntohl(v);
    return 0;
}

int read_bytes(reader_t* r, void* dest, size_t n) {

    if (r->pos + n > r->size)
        return 1;

    memcpy(dest, r->data + r->pos, n);
    r->pos += n;

    return 0;
}