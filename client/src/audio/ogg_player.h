/**
 * @file        ogg_player.h
 * @brief       Ogg file reading and streaming
 */

#ifndef OGG_PLAYER
#define OGG_PLAYER

// Implementation based on https://github.com/vlang/v/blob/master/examples/sokol/sounds/ogg_player.v

#define STB_VORBIS_HEADER_ONLY
#include "stb_vorbis.c" // I DIDNT KNOW YOU COULD DO THIS !????

typedef struct {
    int channels;
    int sample_rate;

    int pos;
    char finished;

    int push_slack_ms;

    unsigned int stream_rate;
    int stream_channels;
    unsigned int stream_len_samples;
    float stream_len_seconds;

    stb_vorbis_alloc* alloc;
    stb_vorbis* vorbis;
} ogg_audio_player;

void audio_init(ogg_audio_player* player, int push_slack_ms);
void audio_destroy(ogg_audio_player* player);

void stream_ogg_file(ogg_audio_player* player, const char* path);

#endif