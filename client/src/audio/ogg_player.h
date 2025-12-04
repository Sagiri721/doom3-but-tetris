/**
 * @file        ogg_player.h
 * @brief       Ogg file reading and streaming
 */

#ifndef OGG_PLAYER
#define OGG_PLAYER

// Implementation based on https://github.com/vlang/v/blob/master/examples/sokol/sounds/ogg_player.v

#define STB_VORBIS_HEADER_ONLY
#include "stb_vorbis.c" // I DIDNT KNOW YOU COULD DO THIS !????

#include "../lib/tinycthread.h"

typedef struct {
    int channels;
    unsigned int sample_rate;

    int pos;
    char finished;
    char loop;

    int push_slack_ms;

    unsigned int stream_rate;
    int stream_channels;
    unsigned int stream_len_samples;
    float stream_len_seconds;

    stb_vorbis_alloc* alloc;
    stb_vorbis* vorbis;

    // The thread that streams audio
    thrd_t audio_thread;
    mtx_t mutex;
    cnd_t cond;

    volatile char should_stop;
    char running;

} ogg_audio_player;

void audio_init(ogg_audio_player* player, int push_slack_ms, char loop);
void audio_destroy(ogg_audio_player* player);

void push_ogg_file(ogg_audio_player* player, const char* path);

#endif