/**
 * @file        ogg_player.c
 * @brief       Ogg Vorbis audio player
 */

#include "ogg_player.h"

#include "../sokol_gp/thirdparty/sokol_audio.h"

#include <stdio.h>
#include <time.h>
#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

void close_decoder(ogg_audio_player* player) {
    if (player->vorbis) {
        stb_vorbis_close(player->vorbis);
        player->vorbis = NULL;
    }
}

void stream_ogg_file(ogg_audio_player* player, const char* path) {
    
    close_decoder(player);

    // Load ogg file
    player->vorbis = stb_vorbis_open_filename(path, NULL, NULL);
    if (!player->vorbis) {
        fprintf(stderr, "Failed to open Ogg Vorbis file: %s\n", path);
        exit(1);
    }

    player->finished = 0;
    player->pos = 0;

    // Inform player of file info
    stb_vorbis_info info = stb_vorbis_get_info(player->vorbis);
    player->stream_rate = info.sample_rate;
    player->stream_channels = info.channels;
    player->stream_len_samples = stb_vorbis_stream_length_in_samples(player->vorbis);
    player->stream_len_seconds = stb_vorbis_stream_length_in_seconds(player->vorbis);

    if (!(
        player->channels == player->stream_channels && 
        player->sample_rate == player->stream_rate
    )) {
        saudio_shutdown();
        saudio_setup(&(saudio_desc){
            .sample_rate = player->stream_rate,
            .num_channels = player->stream_channels
        });

        player->sample_rate = saudio_sample_rate();
        player->channels = saudio_channels();
    }

    printf("Playing Ogg Vorbis file: rate=%d channels=%d length=%.2f seconds\n", 
        player->stream_rate, 
        player->stream_channels, 
        player->stream_len_seconds
    );

    float frames_buffer[4096 * 2];
    float* buffer = frames_buffer;

    while (!player->finished)
    {
        int delay = player->push_slack_ms;
        int expected_frames = saudio_expect();
        if (expected_frames > 0) {
            int decoded_frames = 0;
            while (decoded_frames < expected_frames) {
                
                int samples = stb_vorbis_get_samples_float_interleaved(
                    player->vorbis, 
                    player->channels, 
                    buffer,
                    1024
                );

                if (samples == 0) {
                    player->finished = 1;
                    break;
                }

                int written_frames = saudio_push(
                    buffer,
                    samples
                );
                decoded_frames += written_frames;
                player->pos += samples;
            }
            
            delay = (1000 * decoded_frames) / player->sample_rate;
        }

        printf("\r position: %.2f / %.2f seconds ", 
            (float)player->pos / player->sample_rate, 
            player->stream_len_seconds
        );
    }
    
}

void audio_init(ogg_audio_player* player, int push_slack_ms) {

    player->sample_rate = saudio_sample_rate();
    player->channels = saudio_channels();
    player->push_slack_ms = push_slack_ms;

    size_t alloc_size = 200 * 1024; //? why
    player->alloc = &(stb_vorbis_alloc){
        .alloc_buffer = calloc(1, alloc_size),
        .alloc_buffer_length_in_bytes = alloc_size
    };
}

void audio_destroy(ogg_audio_player *player) {

    player->finished = 1;
    player->pos = 0;

    close_decoder(player);
    free(player->alloc->alloc_buffer);
}