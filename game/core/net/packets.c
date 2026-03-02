/**
 * @file        packets.c
 * @brief       Net packet serialization and deserialization
 */

#include "packets.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>

type_desc_t types[] = {
    PACKET_TYPES_ITER(DO_TYPE_DESC)
};

void serialize_field(buffer_t* buffer, const field_desc_t* field, void* base) {

    // Calculate pointer to field
    void* field_ptr = (char*)base + field->offset;

    // Whatever bro
    uint32_t v;
    float v2;
    const char* str;

    switch (field->type)
    {
        case FIELD_TYPE_INT:
            v = *(uint32_t*)field_ptr;
            write_u32(buffer, v);
            break;
        case FIELD_TYPE_FLOAT:
            v2 = *(float*)field_ptr;
            write_bytes(buffer, &v2, sizeof(float));
            break;
        case FIELD_TYPE_STR:
            str = *(const char**)field_ptr;
            uint16_t len = (uint16_t) strlen(str);

            write_u16(buffer, len);
            write_bytes(buffer, str, len);
            break;
    }
}

void serialize_packet(buffer_t* buffer, packet_types_t* packet) {

    // Write packet type first
    write_u16(buffer, (short) packet->type);

    const type_desc_t* desc = &types[packet->type];
    for (size_t i = 0; i < 16 /*Hard coded limit from struct*/ && desc->fields[i].name; i++)
    {
        serialize_field(buffer, &desc->fields[i], packet);
    }
}

// TODO: Handle errors here
// Auto disconnect maybe
void deserialize_field(reader_t* buffer, const field_desc_t* field, void* base)
{
    void* field_ptr = (char*)base + field->offset;

    uint32_t v;
    float v2;
    char* str;
    uint16_t len;

    switch (field->type)
    {
        case FIELD_TYPE_INT:
            
            read_u32(buffer, &v);
            *(int*)field_ptr = v;

            break;
        case FIELD_TYPE_FLOAT:

            read_bytes(buffer, &v2, sizeof(float));        
            *(float*)field_ptr = v2;
            
            break;
        case FIELD_TYPE_STR:

            read_u16(buffer, &len);
            char* str = (char*) malloc(len + 1);
            assert(str);

            read_bytes(buffer, str, len);
            str[len] = '\0';

            *(char**)field_ptr = str;
                        
            break;
    }
}

void deserialize_packet(reader_t* buffer, packet_types_t* out)
{

    // Read packet type
    uint16_t pt;
    read_u16(buffer, &pt);

    out->type = (packet_type) pt;

    const type_desc_t* desc = &types[pt];
    for (size_t i = 0; i < 16 && desc->fields[i].name; i++) {
        deserialize_field(buffer, &desc->fields[i], out);
    }
}